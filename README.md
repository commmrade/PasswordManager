# PasswordManager
This is literally called "Password Manager", yes.
It is an application that lets you securely store passwords and back them up on the cloud, which you can self-host.
Built with Qt.

### How does it work 
Once you open up the app for the first time, you are given a **master** password, which you should securely store somewhere. Master password is a password, that is used to encrypt/decrypt sensitive data (passwords in this case). If the master password is lost and you are not using it right now in your app installation, you can safely assume that you've lost the data (passwords to be exact).
Master passwords are not uploaded to the cloud, so there is really no way to recover it, unless you are some godly nasty hackeh.  

Also, worth mentioning, PasswordManager supports 2 types of GUI - one is made with QtWidgets, the other one is made with QtQuick (It's kinda nicer tho).
By default, QtWidgets variant is used. You can change it in the settings, it is literally in the first row. (From what I remember, and I'm not double-checking it) 

Data is stored as records in SQlite database (.pm files).

### Backend
- https://github.com/commmrade/PasswordManager-Backend
That's the backend you should use for self-hosting. Not a piece of art, but usable, and you can fork it anytime and improve it.
It consists of 2 big main juicy fat modules: users part (auth, user info and etc) and storage part (uploading and downloading).
Techonologies used:
- Rust
- MinIO Object Storage
- MariaDB database (or MySql, since MariaDB and MySql are interchangable (i think, again, I'm not double-checking that))

### How to build, deploy and yada yada
First, you need to build the Qt project

Dependencies needed:
- gcc clang git yada yada u know basic stuff
- crypto++ (cryptopp)
- qt
- docker to build a backend image

1. I think you need to clone it
```bash
git clone https://github.com/commmrade/PasswordManager
```
2. Then build it
```bash
mkdir build && cd build
cmake ..
make install
```