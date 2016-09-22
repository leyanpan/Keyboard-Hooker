# Hook
A keyboard hooker that sends keystroke on windows via email

# Usage
1. Create a file in the same directory named swap.sys
2. Compile the project (you would need Visual Studio) or run the pre-compiled exe file in release. The program will run in background.
3. The program will send a mail every 3 minutes if there is any stroke recorded. Check your mailbox regularly!
4. To stop, enter Ctrl + `, it will stop automatically.

#File format
The swap.sys should be organized in the following format:

1st Line: The server that the mail request should be sent to e.g. smtp.163.com

2nd Line: Mail address you want to use e.g. xxx@163.com

3rd Line: Mailbox password e.g. 123456

The file should contain the following for the example:

smtp.163.com

xxx@163.com

123456

The mails will be sent from and to the mailbox entered.

# Problems
The process is still visible through task manager.
And there should be a lot of problems to solve!
