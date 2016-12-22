import javax.bluetooth.LocalDevice;
import javax.bluetooth.RemoteDevice;
import javax.bluetooth.UUID;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.io.StreamConnectionNotifier;
import java.awt.*;
import java.awt.event.InputEvent;
import java.io.*;

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

        OutputStream outStream = connection.openOutputStream();
        PrintWriter pWriter = new PrintWriter(new OutputStreamWriter(outStream));

        Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
        pWriter.print(String.format("x %d", (int) screen.getWidth()));
        pWriter.flush();
        pWriter.print(String.format("y %d", (int) screen.getHeight()));
        pWriter.flush();
        System.out.println("write screen size : " + screen.getWidth() + "x" + screen.getHeight());

        String lineRead;
        while ((lineRead = bReader.readLine()) != null) {
            System.out.println(lineRead);
            String[] cmds = lineRead.split(" ");
            String opcode = cmds[0];

            switch (opcode) {
                // move mouse
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
                // mouse press
                case "p": {
                    String finger = cmds[1];
                    if (finger.startsWith("r")) {
                        robot.mousePress(InputEvent.BUTTON3_MASK);
                        System.out.println("Press right mouse");
                    } else if (finger.startsWith("l")) {
                        robot.mousePress(InputEvent.BUTTON1_MASK);
                        System.out.println("press left mouse");
                    }
                    break;
                }
                // mouse release
                case "r": {
                    String finger = cmds[1];
                    if (finger.startsWith("r")) {
                        robot.mouseRelease(InputEvent.BUTTON3_DOWN_MASK);
                        System.out.println("Release right mouse");
                    } else if (finger.startsWith("l")) {
                        robot.mouseRelease(InputEvent.BUTTON1_DOWN_MASK);
                        System.out.println("Release left mouse");
                    }
                    break;
                }
                // screen size
                case "s": {
                    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
                    pWriter.print(String.format("x %d", (int) screenSize.getWidth()));
                    pWriter.flush();
                    pWriter.print(String.format("y %d", (int) screenSize.getHeight()));
                    pWriter.flush();
                    System.out.println("write screen size : " + screenSize.getWidth() + "x" + screenSize.getHeight());
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