# Project Issues and Fixes

## Problems to Fix

### INVITE Command ✔️ "MOSTLY DONE"
The INVITE command implementation is almost complete. It correctly handles sending invitations and ensuring that invited users are added to the `invitedMembers` list for the specified channel. However, there are minor details to refine, such as ensuring that duplicate invitations are not sent and providing appropriate feedback to the inviter if the invitee is already on the invite list or already in the channel.

### MODE Command ✔️ "MOSTLY DONE"
The MODE command is largely implemented. It manages channel modes and user modes, allowing for the configuration of channel restrictions and user privileges. Remaining tasks include fine-tuning specific mode operations, ensuring modes are properly enforced, and enhancing feedback messages to align with the IRC protocol standards.

### TOPIC Command ❌
The TOPIC command is currently not implemented. This command allows users to set or view the topic of a channel. Implementation should ensure that only users with the necessary privileges (e.g., channel operators) can change the topic, and that the topic is properly stored and displayed to users who join the channel.

### -o Parameter in MODE Command Should Not Include Creator of the Channel ✔️
This issue has been resolved. The MODE command has been updated to exclude the channel creator from being affected by the `-o` parameter, which removes operator status. The creator remains an operator by default, and their status is protected from this specific mode change.

### +l Limit User Args Check to be Int and More Than What Already Exists in Channel ✔️
The issue with the `+l` mode, which sets a user limit for the channel, has been fixed. The implementation now checks that the user-supplied limit is a valid integer and ensures it is greater than the current number of users in the channel. This prevents setting a limit that would immediately exclude existing users.

### User Can Authenticate Just with USER Command, Without Password Nor Nick ❌
This security issue is yet to be resolved. Currently, a user can authenticate with the USER command alone, bypassing the need for a password or nickname. This violates the typical IRC authentication flow, which requires a valid password and nickname. The fix will involve enforcing stricter checks to ensure both a password and nickname are provided and validated during the authentication process.

### JOIN Check if Client is in InvitedMembers Vector if Channel is Invite Only ❌
The JOIN command is not currently verifying whether a client is in the `invitedMembers` list for channels that are set to invite-only. This needs to be addressed by modifying the JOIN command logic to include a check that confirms the client is on the invitation list before allowing them to join an invite-only channel.

### Delete Client from InvitedMembers When Joining ❌
Upon a successful join, the client should be removed from the `invitedMembers` list to prevent them from using the same invitation to rejoin after leaving. This behavior is not yet implemented and needs to be added to ensure proper management of the invitation list and to avoid potential abuses of the invite system.

### Upon joining a channel, the topic should be anounced ❌


### Topic Command Issues ✔️
when topic is set with a `':'`, it ignores spaces, 
the topic reply doesn't include the topicSetter, 
the topic reply time Set is not right 100%, 
SPACE AFTER `":"` IN TOPIC MESSAGE SHOULD BE REMOVED, 
broadcasting the message to clients result a `broken pipe error`.``



## New:
Private message command send two messages for the same client.