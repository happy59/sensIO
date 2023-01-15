# SensIO
Here's my upgrade for the Philips Senso. *It should work for any other coffee machine that use push buttons likewise*.
The upgrade is easily reversible. Feel free to take and upgrade this project !
3D print files are available at  [INSERT PRINTABLES LINK](lol%20prout).

I based this project out of a D1miniPro but it can be made with a classic NodeMCU or another ESP board with similar component *(be carefull the D1Mini, even if it has a 5v output, runs on 3,3v)*
## How it works
![UI](sensIO%20UI.png)
Using this interface you can :
 - Know if there's a cup
 - Stirr your cup
 - Turn on and off your senseo
 - brew a little or a large cup
 - know the coffee temperature
 - setup a timer for your coffee to be made

## What you need

 - Wemos D1miniPro
 - Few cables
 - 4 relay modules (3v coil)
 - 4 SOT23 A09T MOSFETs
 - 4 diodes
 - Few resistors
	 - 4x100K
	 - 4x1K
	 - 2x10K
 - DIY PCB
	 - If you can't engrave it yourself I suggest you to use a classic breadboard, replacing SOT23 with long legs mosfets
 - 5v source to power the D1mini
	 - I use an old phone charger
 - DC Motor (5V)
 - DS1820 waterproof temperature sensor
 - Sharp IR distance sensor
	 - It's absolutly overkill, any 1/0 laser braking sensor should work
 - 3D printed parts
 - 4 neodymium magnets
 - Few M3 screws
 - Rubber band
## How to do it
Prepare your PCB by soldering all the parts.
This PCB takes place in the spilled coffee tank, you have to drill some holes in it to wire the three buttons. **Don't forget to flash arduino code**, arduino will still be accessible but you'll have to re-open the machine's back to flash it.

Now you can solder wires to the buttons, these welds are pretty easy to do but **be carefull to unplug the machine** the big shiny plate is full of painfull high voltage (trust me, it is). Don't forget the motor cables (not like me), drill through the printed plate.

don't forget thermometer and IR sensor cables, take them to the back. Now you can close the bottom of the machine. Drill a hole for the thermometer and a window to screw the cup sensor. 
I made Y connections from 220v. One branch to the machine and the other to the phone charger. We'll put the phone charger right behind the watertank (safety first 🥴). I wrapped it in thermal sheath to isolate it.

You can put back all the part in place and look at the 3D printed parts.
Put the magnets in their housing. Add a rubber band and tight the magnet pulley in place with M3 screws. And glue the motor with his pulley in place.

Now you'll need to find the machine's IP. You can scan your wifi network with ugly windows commands. I suggest you tu use your rooter interface to find it.
Once you found it *(something like 192.168.X.XX)*, paste the ip in your browser and create a shortcut to it on your phone's desktop. You'll have access to the interface. Et voilà !

I suggest you to create a static IP for your machine. Otherwise, ip can change at each reboot of the rooter. You also can create an aliases for this ip. For exemple when you type "sensIO/" in the browser it'll redirect it to the ip.

