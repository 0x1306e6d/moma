import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.UUID;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;
import java.awt.*;
import java.awt.event.InputEvent;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Class that implements an SPP Server which accepts single line of
 * message from an SPP client and sends a single line of response to the client.
 * <p>
 * from http://www.jsr82.com/jsr-82-sample-spp-server-and-client/
 * from http://naver.me/GJsVXO0w
 */
public class MouseAgent {

    //start server
    private void startServer() throws IOException, AWTException {
        //Create a UUID for SPP
        UUID uuid = new UUID("1101", true);
        //Create the servicve url
        String connectionString = "btspp://localhost:" + uuid + ";name=Sample SPP Server";

        //open server url
        StreamConnectionNotifier streamConnNotifier = (StreamConnectionNotifier) Connector.open(connectionString);

        //Wait for client connection
        System.out.println("\nServer Started. Waiting for clients to connect...");
        StreamConnection connection = streamConnNotifier.acceptAndOpen();

        RemoteDevice dev = RemoteDevice.getRemoteDevice(connection);
        System.out.println("Remote device address: " + dev.getBluetoothAddress());
        System.out.println("Remote device name: " + dev.getFriendlyName(true));

        Robot robot = new Robot();

        //read string from spp client
        InputStream inStream = connection.openInputStream();
        BufferedReader bReader = new BufferedReader(new InputStreamReader(inStream));

        String lineRead;
        while ((lineRead = bReader.readLine()) != null) {
            System.out.println(lineRead);
            String[] cmds = lineRead.split(" ");
            String opcode = cmds[0];

            switch (opcode) {
                case "m": {
                    int x = Integer.parseInt(cmds[1]);
                    int y = Integer.parseInt(cmds[2]);

                    robot.mouseMove(x, y);
                    System.out.println("Move to " + x + ", " + y);
                    break;
                }
                case "c": {
                    String finger = cmds[1];
                    if (finger.startsWith("r")) {
                        robot.mousePress(InputEvent.BUTTON3_MASK);
                        robot.mouseRelease(InputEvent.BUTTON3_DOWN_MASK);
                        System.out.println("Click right");
                    } else if (finger.startsWith("l")) {
                        robot.mousePress(InputEvent.BUTTON1_MASK);
                        robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
                        System.out.println("Click left");
                    }
                    break;
                }
            }
        }

        bReader.close();
        streamConnNotifier.close();
    }

    public static void main(String[] args) throws IOException, AWTException {
        //display local device address and name
        LocalDevice localDevice = LocalDevice.getLocalDevice();
        System.out.println("Address: " + localDevice.getBluetoothAddress());
        System.out.println("Name: " + localDevice.getFriendlyName());

        MouseAgent mouseAgent = new MouseAgent();
        mouseAgent.startServer();
    }
}