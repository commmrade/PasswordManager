# PasswordManager
This Password Manager is written in C++ with the help of QT.
It uses powerful encryption to store passwords, so it is really hard to crack


### How does it work in general
Once you open up the app for the first time you are given a **master** password, which you really should save somewhere. It is used to encrypt and decrypt passwords.
Also, when exporting your storage for a use on another device you should remember to use correct password (not applied when using cloud saves - it is done there automatically).
Under the hood, each note is stored inside SQLite database with additional encryption.

Worth mentioning, that there are 2 types of GUI available to users - made with Widgets and with Qt Quick 2 (better btw)

### Backend
- https://github.com/commmrade/PasswordManager-Backend
Backend here is a monolithical piece of real good engineering. There is 2 main parts, which are a module for storage managing (cloud saves basically) and a module for authentication.
Refresh token + JWT token technology is used here to make sure if someone steals your JWT Token, it will be expired pretty soon, but you don't have to reauthenticate, because refresh token comes to help

