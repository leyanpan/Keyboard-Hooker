# Hook
A keyboard hooker that sends keystroke on windows via email

# Usage
1. Modify the "SendMail" function in KHook.cpp so that the mail, password and server matches the mailbox that you want to receive and send the keystroke
2. Compile the project (you would need Visual Studio) and run. The program will run in background.
3. The program will send a mail every 3 minutes if there is any stroke recorded. Check your mailbox regularly!
4. To stop, enter Ctrl + `, it will stop automatically.

# Problems
The process is still visible through task manager
And there should be a lot of problems to solve!
