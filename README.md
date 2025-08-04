# Brick Rigs Server Director

Overview:  
Brick Rigs Server Director is an injected mod for brick rigs that adds various administrative and creative additions to help manage your brick rigs server.

## Working On
 - Blackist
 - Admin Elevator
 - Saver

## Planning:

1. Full functionality of the Brick Rigs Command Interpreter.  
   1. All commands of BRCI  
   2. add additional commands  
   3. Add a gui to help manage commands   
   4. Add scripting to create your own commands  
2. Fully fledged accompanying GUI  
   1. Startup GUI  
   2. Add additional GUI modules to existing GUIS (like server creator or game menu) for more control  
   3. Add In-Game overlay for relevant information and others   
3. Additional server management   
   1. Ties into vanilla implementations  
   2. Add in-game game mode modifications   
   3. Add banned bricks   
   4. Blacklisted vehicles via steam links 
   5. Banned vehicles using file detection  
   6. “Saver” to save vehicle placements and load/save them  
4.   
5. Additions to existing game modes(toggleable)(Works on vanilla roles)  
   1. Roleplay  
      1. Speed Limits  
      2. 911 System  
      3.   
6. Expectations   
   1. Durable(change word)  
      1. Limited/None unexpected behavior.  
      2. Limited/None crashes  
      3. Limit tested and tolerating (user error cannot be the reason for issues  
      4. Non-Messy codebase  
      5. Can be injected/uninjected at any time  
      6. Hook based control instead of loop based   
      7. Prioritize signatures for hooks/calls  
   2. Intuitive   
      1. Intuitive GUIS, chat commands, explanations etc  
      2. Can be figured out visually or with limited context   
      3. No confusing menus/systems  
      4. Abstraction of systems  
         1. Relegate control to BRSD for finite control of systems   
         2. The user doesn't always need full control   
         3. Present in places where the user can cause systems issues   
      5. User base would be young and unintelligible   
   3. Server Side  
      1. No modifications to the client   
      2. No RCE’s on the client  
      3. No client to server vulnerabilities.
