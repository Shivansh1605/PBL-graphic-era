import tkinter as tk
from tkinter import messagebox, simpledialog, ttk
import os, json, csv, random, smtplib, ssl, time
from email.message import EmailMessage
from pathlib import Path

# Optional OpenCV dependency for face capture/verification
try:
    import cv2
    import numpy as np
except ImportError:
    cv2 = None
    np = None

APP_DIR = Path(__file__).resolve().parent
VOTERS_FILE = APP_DIR / "voters.txt"
CANDIDATES_FILE = APP_DIR / "candidates.txt"
EMAILS_FILE = APP_DIR / "voter_emails.json"
EMAIL_CONFIG_FILE = APP_DIR / "email_config.json"
AUDIT_FILE = APP_DIR / "gui_audit.log"
FACES_DIR = APP_DIR / "faces"
ADMIN_PASSWORD = "admin123"

BG = "#1E293B"
DARK = "#0B1F3A"
BLUE = "#2563EB"
CYAN = "#00A8E8"
WHITE = "#FFFFFF"
MUTED = "#B8C7D9"


def read_voters():
    voters = {}
    if VOTERS_FILE.exists():
        with open(VOTERS_FILE, "r", encoding="utf-8") as f:
            for line in f:
                parts = line.strip().split("|")
                if len(parts) >= 4:
                    voters[parts[0]] = {
                        "name": parts[1], "pin": parts[2], "voted": parts[3] == "1"
                    }
    return voters


def write_voters(voters):
    with open(VOTERS_FILE, "w", encoding="utf-8") as f:
        for vid, data in voters.items():
            f.write(f'{vid}|{data["name"]}|{data["pin"]}|{1 if data["voted"] else 0}\n')


def read_candidates():
    candidates = []
    if CANDIDATES_FILE.exists():
        with open(CANDIDATES_FILE, "r", encoding="utf-8") as f:
            for line in f:
                p = line.strip().split("|")
                if len(p) >= 3:
                    try:
                        candidates.append({"id": int(p[0]), "name": p[1], "votes": int(p[2])})
                    except ValueError:
                        pass
    if not candidates:
        candidates = [{"id": 1, "name": "Option_A", "votes": 0},
                      {"id": 2, "name": "Option_B", "votes": 0}]
        write_candidates(candidates)
    return candidates


def write_candidates(candidates):
    with open(CANDIDATES_FILE, "w", encoding="utf-8") as f:
        for c in candidates:
            f.write(f'{c["id"]}|{c["name"]}|{c["votes"]}\n')


def read_emails():
    try:
        return json.loads(EMAILS_FILE.read_text(encoding="utf-8"))
    except Exception:
        return {}


def write_emails(data):
    EMAILS_FILE.write_text(json.dumps(data, indent=2), encoding="utf-8")


def audit(message):
    stamp = time.strftime("%Y-%m-%d %H:%M:%S")
    with open(AUDIT_FILE, "a", encoding="utf-8") as f:
        f.write(f"{stamp} | {message}\n")


def email_config():
    try:
        return json.loads(EMAIL_CONFIG_FILE.read_text(encoding="utf-8"))
    except Exception:
        return {}


def send_otp_email(destination, otp):
    cfg = email_config()
    sender = cfg.get("sender", "").strip()
    app_password = cfg.get("app_password", "").replace(" ", "")
    if not sender or not app_password:
        raise RuntimeError("Email SMTP is not configured. Use Settings → Email OTP Setup first.")
    msg = EmailMessage()
    msg["Subject"] = "VoteSecure verification code"
    msg["From"] = sender
    msg["To"] = destination
    msg.set_content(f"Your VoteSecure OTP is: {otp}\nIt expires in 5 minutes. Do not share it.")
    context = ssl.create_default_context()
    with smtplib.SMTP_SSL("smtp.gmail.com", 465, context=context, timeout=20) as server:
        server.login(sender, app_password)
        server.send_message(msg)


def haar_detector():
    if cv2 is None:
        raise RuntimeError("OpenCV is not installed. Run: python -m pip install opencv-contrib-python")
    cascade_path = cv2.data.haarcascades + "haarcascade_frontalface_default.xml"
    detector = cv2.CascadeClassifier(cascade_path)
    if detector.empty():
        raise RuntimeError("Could not load OpenCV face detector.")
    return detector


def normalize_face(gray, x, y, w, h):
    face = gray[y:y+h, x:x+w]
    face = cv2.resize(face, (200, 200))
    return face


def capture_enrollment_faces(voter_id):
    if cv2 is None:
        raise RuntimeError("Install OpenCV first: python -m pip install opencv-contrib-python")
    detector = haar_detector()
    folder = FACES_DIR / voter_id
    folder.mkdir(parents=True, exist_ok=True)
    # Clear any partial previous registration images for this ID.
    for old in folder.glob("*.png"):
        old.unlink()

    cam = cv2.VideoCapture(0)
    if not cam.isOpened():
        raise RuntimeError("Could not open webcam. Check camera permissions.")
    saved = 0
    try:
        while saved < 5:
            ok, frame = cam.read()
            if not ok:
                continue
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = detector.detectMultiScale(gray, 1.2, 5, minSize=(100, 100))
            for (x, y, w, h) in faces:
                cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
            cv2.putText(frame, f"Enrollment: {saved}/5 | SPACE capture | Q cancel",
                        (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255,255,255), 2)
            cv2.imshow("VoteSecure - Face Enrollment", frame)
            key = cv2.waitKey(1) & 0xFF
            if key == ord("q"):
                break
            if key == 32 and len(faces) > 0:
                x, y, w, h = max(faces, key=lambda r: r[2]*r[3])
                face = normalize_face(gray, x, y, w, h)
                cv2.imwrite(str(folder / f"{saved}.png"), face)
                saved += 1
                time.sleep(0.25)
    finally:
        cam.release()
        cv2.destroyAllWindows()
    if saved != 5:
        for old in folder.glob("*.png"):
            old.unlink()
        raise RuntimeError("Face enrollment cancelled. Please capture all 5 samples.")
    return True


def verify_face(voter_id):
    if cv2 is None:
        raise RuntimeError("Install OpenCV first: python -m pip install opencv-contrib-python")
    if not hasattr(cv2, "face"):
        raise RuntimeError("OpenCV face module missing. Install opencv-contrib-python (not opencv-python).")
    detector = haar_detector()
    face_images, labels = [], []
    ids = []
    for folder in FACES_DIR.iterdir() if FACES_DIR.exists() else []:
        if folder.is_dir():
            images = list(folder.glob("*.png"))
            if images:
                ids.append(folder.name)
    ids = sorted(ids)
    if voter_id not in ids:
        return False
    id_to_label = {vid: i for i, vid in enumerate(ids)}
    for vid in ids:
        for path in (FACES_DIR / vid).glob("*.png"):
            img = cv2.imread(str(path), cv2.IMREAD_GRAYSCALE)
            if img is not None:
                face_images.append(cv2.resize(img, (200, 200)))
                labels.append(id_to_label[vid])
    if not face_images:
        return False
    model = cv2.face.LBPHFaceRecognizer_create()
    model.train(face_images, np.array(labels, dtype=np.int32))

    cam = cv2.VideoCapture(0)
    if not cam.isOpened():
        raise RuntimeError("Could not open webcam.")
    matched = False
    try:
        start = time.time()
        while time.time() - start < 30:
            ok, frame = cam.read()
            if not ok:
                continue
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            faces = detector.detectMultiScale(gray, 1.2, 5, minSize=(100, 100))
            for (x, y, w, h) in faces:
                cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,0), 2)
            cv2.putText(frame, "Look at camera | Q cancel", (10,30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,255,255), 2)
            cv2.imshow("VoteSecure - Face Verification", frame)
            key = cv2.waitKey(1) & 0xFF
            if key == ord("q"):
                break
            if len(faces):
                x, y, w, h = max(faces, key=lambda r:r[2]*r[3])
                face = normalize_face(gray,x,y,w,h)
                predicted, confidence = model.predict(face)
                # LBPH distance: lower is a closer match. Threshold is demo-oriented.
                if predicted == id_to_label[voter_id] and confidence < 80:
                    matched = True
                    break
                # Don't instantly reject: allow user to adjust camera for up to 30 sec.
    finally:
        cam.release()
        cv2.destroyAllWindows()
    return matched


class VoteSecureApp:
    def __init__(self, root):
        self.root = root
        root.title("VoteSecure - Digital Democracy Voting System")
        root.geometry("900x650")
        root.resizable(False, False)
        root.configure(bg=BG)
        self.build_home()

    def clear(self):
        for widget in self.root.winfo_children():
            widget.destroy()

    def build_home(self):
        self.clear()
        tk.Label(self.root, text="VoteSecure", font=("Arial", 34, "bold"),
                 fg=WHITE, bg=BG).pack(pady=(25, 5))
        tk.Label(self.root, text="Digital Democracy Voting System",
                 font=("Arial", 16), fg="#D1D5DB", bg=BG).pack(pady=5)
        tk.Label(self.root, text="Secure • Transparent • Reliable",
                 font=("Arial", 12), fg=CYAN, bg=BG).pack(pady=8)

        frame = tk.Frame(self.root, bg=BG)
        frame.pack(pady=25)

        buttons = [
            ("Voter Registration", self.registration),
            ("Voter Login", self.voter_login),
            ("Cast Vote", self.cast_vote),
            ("View Candidates", self.show_candidates),
            ("Admin Login", self.admin_login),
            ("View Results", self.view_results),
        ]
        for i, (text, command) in enumerate(buttons):
            tk.Button(frame, text=text, command=command, font=("Arial", 12, "bold"),
                      bg=BLUE, fg=WHITE, activebackground="#1D4ED8",
                      width=24, height=2, cursor="hand2").grid(
                          row=i//2, column=i%2, padx=15, pady=12)

        menu = tk.Menu(self.root)
        settings = tk.Menu(menu, tearoff=0)
        settings.add_command(label="Email OTP Setup", command=self.email_setup)
        settings.add_command(label="View Audit Log", command=self.view_audit)
        menu.add_cascade(label="Settings", menu=settings)
        self.root.config(menu=menu)
        tk.Label(self.root, text="VoteSecure | Student Project",
                 font=("Arial", 10), fg="#94A3B8", bg=BG).pack(side="bottom", pady=15)

    def email_setup(self):
        win = tk.Toplevel(self.root)
        win.title("Email OTP Setup")
        win.geometry("480x300")
        win.configure(bg=DARK)
        cfg = email_config()
        tk.Label(win, text="Gmail SMTP configuration", font=("Arial", 16, "bold"),
                 bg=DARK, fg=WHITE).pack(pady=12)
        tk.Label(win, text="Gmail address", bg=DARK, fg=WHITE).pack()
        sender = tk.Entry(win, width=42)
        sender.pack(pady=4)
        sender.insert(0, cfg.get("sender", ""))
        tk.Label(win, text="Google App Password (not your Gmail password)",
                 bg=DARK, fg=WHITE).pack()
        password = tk.Entry(win, width=42, show="*")
        password.pack(pady=4)
        password.insert(0, cfg.get("app_password", ""))

        def save():
            if "@" not in sender.get() or not password.get().strip():
                messagebox.showerror("Error", "Enter a valid Gmail and App Password.", parent=win)
                return
            EMAIL_CONFIG_FILE.write_text(json.dumps({
                "sender": sender.get().strip(),
                "app_password": password.get().strip()
            }, indent=2), encoding="utf-8")
            messagebox.showinfo("Saved", "Email settings saved locally.", parent=win)
            win.destroy()
        tk.Button(win, text="Save Settings", command=save, bg=CYAN, fg=WHITE,
                  font=("Arial", 11, "bold")).pack(pady=15)
        tk.Label(win, text="Requires Gmail 2-Step Verification and a Google App Password.",
                 bg=DARK, fg=MUTED, wraplength=420).pack()

    def registration(self):
        win = tk.Toplevel(self.root)
        win.title("Voter Registration")
        win.geometry("500x520")
        win.configure(bg=DARK)
        win.resizable(False, False)
        tk.Label(win, text="Voter Registration", font=("Arial", 22, "bold"),
                 bg=DARK, fg=WHITE).pack(pady=(18, 6))
        fields = {}
        for label in ("Voter ID", "Full Name", "Email Address", "Create PIN", "Confirm PIN"):
            tk.Label(win, text=label, bg=DARK, fg=WHITE).pack(pady=(7, 2))
            entry = tk.Entry(win, width=35, font=("Arial", 11),
                             show="*" if "PIN" in label else "")
            entry.pack()
            fields[label] = entry
        def submit():
            vid = fields["Voter ID"].get().strip()
            name = fields["Full Name"].get().strip().replace(" ", "_")
            email = fields["Email Address"].get().strip()
            pin = fields["Create PIN"].get()
            confirm = fields["Confirm PIN"].get()
            if not all([vid, name, email, pin, confirm]):
                messagebox.showerror("Error", "Fill in every field.", parent=win); return
            if "@" not in email or "." not in email.split("@")[-1]:
                messagebox.showerror("Error", "Enter a valid email address.", parent=win); return
            if not pin.isdigit() or len(pin) < 4:
                messagebox.showerror("Error", "PIN must be at least 4 digits.", parent=win); return
            if pin != confirm:
                messagebox.showerror("Error", "PINs do not match.", parent=win); return
            voters = read_voters()
            if vid in voters:
                messagebox.showerror("Error", "This voter ID is already registered.", parent=win); return
            otp = str(random.randint(100000, 999999))
            otp_sent_at = time.time()
            try:
                send_otp_email(email, otp)
            except Exception as e:
                messagebox.showerror("OTP email failed", str(e), parent=win); return
            entered = simpledialog.askstring("Email OTP", f"OTP sent to {email}. Enter OTP:", parent=win)
            if time.time() - otp_sent_at > 300:
                messagebox.showerror("Error", "OTP expired. Please register again.", parent=win); return
            if entered != otp:
                messagebox.showerror("Error", "Incorrect OTP. Registration cancelled.", parent=win); return
            try:
                messagebox.showinfo("Face Enrollment",
                    "Camera will open. Capture 5 face samples using SPACE.\nPress Q to cancel.", parent=win)
                capture_enrollment_faces(vid)
            except Exception as e:
                messagebox.showerror("Face Enrollment Failed", str(e), parent=win); return
            voters[vid] = {"name": name, "pin": pin, "voted": False}
            write_voters(voters)
            emails = read_emails(); emails[vid] = email; write_emails(emails)
            audit(f"Voter registered: {vid}")
            messagebox.showinfo("Success", "Voter registration completed!", parent=win)
            win.destroy()
        tk.Button(win, text="Send OTP & Register", command=submit, bg=CYAN, fg=WHITE,
                  font=("Arial", 11, "bold"), width=22, pady=7).pack(pady=20)

    def authenticate(self, require_face=True):
        vid = simpledialog.askstring("Voter Login", "Voter ID:", parent=self.root)
        if not vid: return None
        pin = simpledialog.askstring("Voter Login", "PIN:", show="*", parent=self.root)
        if pin is None: return None
        voters = read_voters()
        if vid not in voters or voters[vid]["pin"] != pin:
            audit(f"Voter login failed: {vid}")
            messagebox.showerror("Login failed", "Voter ID or PIN incorrect.", parent=self.root)
            return None
        email = read_emails().get(vid)
        if not email:
            messagebox.showerror("Error", "No email saved for this voter.", parent=self.root); return None
        otp = str(random.randint(100000, 999999))
        otp_sent_at = time.time()
        try:
            send_otp_email(email, otp)
        except Exception as e:
            messagebox.showerror("OTP error", str(e), parent=self.root); return None
        entered = simpledialog.askstring("Email OTP", f"OTP sent to {email}. Enter OTP:", parent=self.root)
        if time.time() - otp_sent_at > 300:
            messagebox.showerror("Error", "OTP expired. Request a new login.", parent=self.root); return None
        if entered != otp:
            audit(f"OTP failed: {vid}")
            messagebox.showerror("Error", "OTP verification failed.", parent=self.root); return None
        if require_face:
            try:
                if not verify_face(vid):
                    audit(f"Face verification failed: {vid}")
                    messagebox.showerror("Error", "Face verification failed.", parent=self.root); return None
            except Exception as e:
                messagebox.showerror("Face verification error", str(e), parent=self.root); return None
        audit(f"Voter authenticated: {vid}")
        return vid

    def voter_login(self):
        vid = self.authenticate(require_face=True)
        if vid:
            messagebox.showinfo("Login successful", f"Welcome, {read_voters()[vid]['name'].replace('_',' ')}!", parent=self.root)

    def cast_vote(self):
        vid = self.authenticate(require_face=True)
        if not vid: return
        voters = read_voters()
        if voters[vid]["voted"]:
            messagebox.showwarning("Already voted", "This voter has already cast a vote.", parent=self.root); return
        candidates = read_candidates()
        win = tk.Toplevel(self.root)
        win.title("Cast Vote")
        win.geometry("450x350")
        win.configure(bg=DARK)
        tk.Label(win, text="Select Candidate / Option", font=("Arial", 16, "bold"),
                 bg=DARK, fg=WHITE).pack(pady=15)
        selection = tk.IntVar(value=0)
        for c in candidates:
            tk.Radiobutton(win, text=f'{c["id"]} - {c["name"]}', variable=selection,
                           value=c["id"], bg=DARK, fg=WHITE, selectcolor=BG,
                           activebackground=DARK).pack(anchor="w", padx=45, pady=5)
        def submit_vote():
            cid = selection.get()
            candidate = next((c for c in candidates if c["id"] == cid), None)
            if candidate is None:
                messagebox.showerror("Error", "Choose a candidate.", parent=win); return
            confirm = messagebox.askyesno("Confirm Vote",
                f"Cast your vote for {candidate['name']}?\nThis cannot be changed.", parent=win)
            if not confirm: return
            candidate["votes"] += 1
            write_candidates(candidates)
            voters[vid]["voted"] = True
            write_voters(voters)
            audit(f"Vote recorded: voter={vid}, candidate={cid}")
            messagebox.showinfo("Vote recorded", "Your vote has been recorded.", parent=win)
            win.destroy()
        tk.Button(win, text="Submit Vote", command=submit_vote, bg=CYAN, fg=WHITE,
                  font=("Arial", 11, "bold")).pack(pady=20)

    def show_candidates(self):
        candidates = read_candidates()
        text = "\n".join(f'{c["id"]}. {c["name"]}' for c in candidates)
        messagebox.showinfo("Candidates / Options", text or "No candidates found.", parent=self.root)

    def admin_login(self):
        password = simpledialog.askstring("Admin Login", "Admin password:", show="*", parent=self.root)
        if password != ADMIN_PASSWORD:
            messagebox.showerror("Admin", "Incorrect admin password.", parent=self.root); return
        audit("Admin login")
        win = tk.Toplevel(self.root)
        win.title("Admin Panel")
        win.geometry("450x350")
        win.configure(bg=DARK)
        tk.Label(win, text="Admin Panel", font=("Arial", 20, "bold"),
                 bg=DARK, fg=WHITE).pack(pady=15)
        tk.Button(win, text="View Results", command=lambda: self.view_results(win),
                  width=25, bg=BLUE, fg=WHITE).pack(pady=8)
        tk.Button(win, text="Add Candidate / Option", command=lambda: self.add_candidate(win),
                  width=25, bg=BLUE, fg=WHITE).pack(pady=8)
        tk.Button(win, text="View Audit Log", command=lambda: self.view_audit(win),
                  width=25, bg=BLUE, fg=WHITE).pack(pady=8)

    def add_candidate(self, parent=None):
        name = simpledialog.askstring("Add Candidate", "Candidate/option name (no | character):", parent=parent or self.root)
        if not name: return
        name = name.strip().replace("|", "").replace("\n", "")
        candidates = read_candidates()
        if not name: return
        new_id = max([c["id"] for c in candidates], default=0) + 1
        candidates.append({"id": new_id, "name": name.replace(" ", "_"), "votes": 0})
        write_candidates(candidates)
        audit(f"Candidate added: {new_id} {name}")
        messagebox.showinfo("Success", f"Added {name} with ID {new_id}.", parent=parent or self.root)

    def view_results(self, parent=None):
        candidates = read_candidates()
        total = sum(c["votes"] for c in candidates)
        lines = ["Election Results", "", f"Total votes: {total}", ""]
        lines += [f'{c["name"]}: {c["votes"]} votes' for c in candidates]
        messagebox.showinfo("Results", "\n".join(lines), parent=parent or self.root)

    def view_audit(self, parent=None):
        try:
            content = AUDIT_FILE.read_text(encoding="utf-8")
        except Exception:
            content = "No audit events yet."
        win = tk.Toplevel(parent or self.root)
        win.title("VoteSecure Audit Log")
        win.geometry("650x400")
        box = tk.Text(win, wrap="word", font=("Consolas", 10))
        box.pack(fill="both", expand=True, padx=10, pady=10)
        box.insert("1.0", content)
        box.config(state="disabled")


if __name__ == "__main__":
    root = tk.Tk()
    app = VoteSecureApp(root)
    root.mainloop()
