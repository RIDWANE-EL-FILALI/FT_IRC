# commands handled by me (JOIN KICK PART PRIVMSG QUIT )


--> authentication : still needs to be tested
--> set nickname: still needs to be tested
--> set username: still needs to be tested
--> join channels:✅
--> send and receive public and private messages using the reference client:✅

--> messages have to be forwarded to every client on the channel:
--> we must have operators and regular users in a channel:✅


--> commands specific to channels :
	- PART:✅
	- PRIVMSG:✅
	- QUIT: ✅
	- KICK :✅
	- INVITE:✅
	- TOPIC:✅
	- MODE: ✅
		-i: Set/remove Invite-only channel
		-t: Set/remove the restrictions of the TOPIC command to channel
			operators
		-k: Set/remove the channel key (password)
		-o: Give/take channel operator privilege
		-l: Set/remove the user limit to channel


the client used in our case is the native to macos irc client limechat so the responses have to follow the order of the limechat client :


# KICK COMMAND :
The `KICK` command is used to remove a user from a channel.
```c++
/KICK <channel> <user> [<comment>]
```
### EXAMPLES : 
```c++
/KICK #chatroom JohnSpammer "Spamming is not allowed"

//This command removes the user `JohnSpammer` from the `#chatroom` channel with the comment "Spamming is not allowed".
```
### SET OF TESTS :
```c++
/KICK #testchannel UserA:✅
/KICK #testchannel UserA "Violation of rules":✅
/KICK #testchannel NonExistentUser:✅
/KICK #testchannel USERA,USERB,NonExistentUser,USERC:✅
/KICK #chennel1,#chennel2,#chennel3 user1,user2,user3:✅
```
# MODE COMMAND:

The `MODE` command is used to change or view the modes of a channel or a user. The specific modes vary, but common channel modes include `i`, `t`, `k`, `o`, and `l`.
### EXAMPLES:
```c++
/MODE <channel> <mode> [<mode parameters>]
/MODE <user> <mode> [<mode parameters>]
```
**Channel Modes and Examples:**

- `i` (Invite-only channel): Only users who are invited can join the channel.
```c++
/MODE #privatechannel +i
```
* `t` (Topic settable by channel operator only): Only channel operators can change the topic.
```c++
/MODE #discussion +t
```
* `k` (Channel key/password): Requires a password to join the channel.
```c++
/MODE #securechannel +k secretpass
```
* `o` (Channel operator status): Grants or removes operator status to/from a user.
```c++
/MODE #chatroom +o JohnDoe
```
* `l` (User limit): Sets a limit on the number of users in the channel.
```c++
/MODE #limitedchannel +l 50
```

### SET OF TESTS:
```c++
1. /MODE #testchannel +i: ✅ // Set #testchannel to invite-only mode
2. /MODE #testchannel -i: ✅  // Remove invite-only mode from #testchannel
3. /MODE #testchannel +t: ✅  // Set #testchannel to topic settable by operators only
4. /MODE #testchannel -t: ✅  // Allow any user to change the topic in #testchannel
5. /MODE #testchannel +k secretkey:✅  // Set a password (secretkey) to join #testchannel
6. /MODE #testchannel -k:✅  // Remove the password requirement from #testchannel
7. /MODE #testchannel +o OperatorUser: ❌  // Give operator status to OperatorUser in #testchannel
8. /MODE #testchannel -o OperatorUser: ❌  // Remove operator status from OperatorUser in #testchannel
11. /MODE #testchannel +l 50:❌   // Set the user limit of #testchannel to 50
12. /MODE #testchannel -l:❌   // Remove the user limit from #testchannel
```


### NOTES : 
```c++

**makram**

-- the topic and invite only restrection works but handle the below cases

--> when a member of a channel enters "MODE #channel"
the server should return the modes of that channel 
--> when a member of a channel enters "TOPIC #channel"
should receive the topic of that channel
--> they're is a pronleme when you set a normal user to be an operator you have to return a valid response for limechat to change it's color to orange in the operator list
--> segfault when you set a user to opertator and take it off and then assign it again to operator (🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴)

-->you have to handle when the user enteres a limite for the channel but it's a number already filled in that channel

-->invite has a probleme when i set the channel as invite only and i invite a user it does not work because he cannot join that channel for some reason (the members(non operators) have to be able to invite users)


**kaoutar**
--> handle password check in sign in (🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴)
--> the user should run no command until he is registered properly
--> check the nickname change too it should take the last one place
-->
```


# TOPIC COMMAND:
The `TOPIC` command is used to set or view the topic of a channel.
```c++
/TOPIC <channel> [<new topic>]
```

### EXAMPLES:
```C++
/TOPIC #discussion "Today's topic is Open Source Software"
```


# INVITE COMMAND:
The `INVITE` command is used to invite a user to a channel. This is often used when the channel is set to invite-only mode.
```c++
/INVITE <user> <channel>
```

### EXAMPLES :
```c++
/INVITE JaneDoe #privatechannel
```


### SET OF TESTS:
```C++
1. /MODE #testchannel +i:  // Set #testchannel to invite-only mode
   /INVITE UserA #testchannel:  // Invite UserA to #testchannel

2. /MODE #testchannel -i:  // Remove invite-only mode from #testchannel
   /INVITE UserB #testchannel:  // Invite UserB to #testchannel (channel is now public)

7. /MODE #testchannel +o OperatorUser:  // Give operator status to OperatorUser in #testchannel
   /INVITE UserG #testchannel:  // OperatorUser invites UserG to #testchannel

8. /MODE #testchannel -o OperatorUser:  // Remove operator status from OperatorUser in #testchannel
   /INVITE UserH #testchannel:  // Invite UserH to #testchannel (by a non-operator)

9. /MODE #testchannel +l 50:  // Set the user limit of #testchannel to 50
   /INVITE UserI #testchannel:  // Invite UserI to #testchannel (assuming the limit is not reached)

10. /MODE #testchannel -l:  // Remove the user limit from #testchannel
    /INVITE UserJ #testchannel:  // Invite UserJ to #testchannel
```


