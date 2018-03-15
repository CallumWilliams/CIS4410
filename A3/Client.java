import javax.swing.SpringLayout;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.net.*;
import java.io.*;

public class Client extends JFrame {
    
	//information variables
	private static String HOST;
	private static Integer PORT;
	private static String USER;
	private static Client c;
	
	//network variables
	private Socket sock;
	private DataInputStream input;
	private DataOutputStream output;
	
	//GUI variables
	private static JLabel logs_label = new JLabel("Chat Logs");
	private static JTextArea chatLogs_field = new JTextArea();
	private static JLabel username_label = new JLabel("Username");
	private static JTextField username_field = new JTextField();
	private static JButton connect_button = new JButton("Connect");
	private static JButton disconnect_button = new JButton("Disconnect");
	private static JTextField sendMessage_field = new JTextField();
	
	/**Sets up client**/
	public Client(String u, String h, Integer p) {
		
		try {
			
			sock = new Socket(h, p);
			input = new DataInputStream(new BufferedInputStream(sock.getInputStream()));
			output = new DataOutputStream(new BufferedOutputStream(sock.getOutputStream()));
			
		} catch (Exception e) {
			System.out.println("Exception caught: " + e);
		}
		
	}
	
	public void readData() {
		
		try {
			
			input.readUTF();
			//process
			
		} catch (Exception e) {
			System.out.println("Exception caught when reading: " + e);
		}
		
	}
	
	public void sendData(String msg) {
		
		try {
			
			output.writeUTF(msg);
			output.flush();
			
		} catch (Exception e) {
			System.out.println("Exception caught when sending '" + msg + "': " + e);
		}
		
	}
	
	private static void loadInterface() {
		
		/**Setup window**/
    	
		JFrame frame = new JFrame("Chat Program");
		frame.setSize(500, 450);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		/**Setup content pane**/
		Container content = frame.getContentPane();
		SpringLayout sl = new SpringLayout();
		content.setLayout(sl);
		
		/**Setup elements**/
		content.add(logs_label);
		
		chatLogs_field.setPreferredSize(new Dimension(250, 375));
		chatLogs_field.setEditable(false);
		content.add(chatLogs_field);
		
		content.add(username_label);
		
		username_field.setPreferredSize(new Dimension(150, 20));
		content.add(username_field);
		
		connect_button.addActionListener(new ActionListener() {
			
			public void actionPerformed(ActionEvent e) {
				String username = username_field.getText();
				if (username != "" && username != null) {
					
					/**establish connection for username**/
                	try {
                		
                		USER = username;
                		c = new Client(USER, HOST, PORT);
                		connect_button.setEnabled(false); disconnect_button.setEnabled(true);
                		
                	} catch (Exception ex) {
                		
                		System.out.println("Exception: " + ex);
                		
                	}
                	
                } else {
                	System.out.println("no username entered");
                }
            }
            
        });
        content.add(connect_button);
        
        disconnect_button.setEnabled(false);
        disconnect_button.addActionListener(new ActionListener() {
            
            public void actionPerformed(ActionEvent e) {
                System.out.println("disconnect");
                //disconnect
                connect_button.setEnabled(true); disconnect_button.setEnabled(false);
            }
            
        });
        content.add(disconnect_button);
        
        sendMessage_field.setPreferredSize(new Dimension(250, 20));
        content.add(sendMessage_field);
        
        /**Put constraints**/
        sl.putConstraint(SpringLayout.NORTH, chatLogs_field, 10, SpringLayout.SOUTH, logs_label);
        sl.putConstraint(SpringLayout.NORTH, username_label, 25, SpringLayout.NORTH, frame);
        sl.putConstraint(SpringLayout.WEST, username_label, 5, SpringLayout.EAST, chatLogs_field);
        sl.putConstraint(SpringLayout.NORTH, username_field, 10, SpringLayout.SOUTH, username_label);
        sl.putConstraint(SpringLayout.WEST, username_field, 10, SpringLayout.EAST, chatLogs_field);
        sl.putConstraint(SpringLayout.NORTH, connect_button, 10, SpringLayout.SOUTH, username_field);
        sl.putConstraint(SpringLayout.WEST, connect_button, 10, SpringLayout.EAST, chatLogs_field);
        sl.putConstraint(SpringLayout.NORTH, disconnect_button, 10, SpringLayout.SOUTH, username_field);
        sl.putConstraint(SpringLayout.WEST, disconnect_button, 10, SpringLayout.EAST, connect_button);
        sl.putConstraint(SpringLayout.NORTH, sendMessage_field, 5, SpringLayout.SOUTH, chatLogs_field);
        
        /**Display**/
        frame.setVisible(true);
        
    }
    
    public static void main(String[] args) {
        
    	if (args.length != 2) {
    		System.out.println("Error - please run as 'java Client <host> <port>");
    		System.exit(0);
    	}
    	HOST = args[0];
		PORT = Integer.parseInt(args[1]);
    	loadInterface();
        
    }
    
}