steps
-------------

Click the DOWNLOADS button in the top right corner, rename the uncompressed folder Spacebrew. Check that the Spacebrew folder contains Spacebrew.cpp and Spacebrew.h

Place the Spacebrew library folder your <arduinosketchfolder>/libraries/ folder. You may need to create the libraries subfolder if its your first library. 

Also download and install the WebSocketClient library from https://github.com/labatrockwell/ArduinoWebsocketClient

Restart the IDE.

includes
--------------

These should be placed at the top of your sketch:

<pre>
#include &lt;Spacebrew.h>
#include &lt;Ethernet.h>
#include &lt;WebSocketClient.h>
</pre>
