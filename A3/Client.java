import javax.swing.SpringLayout;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import java.net.*;
import java.io.*;

public class Client extends JFrame implements Runnable {
    
	//information variables
	private static String HOST;
	private static Integer PORT;
	private static String USER;
	private static Client c;
	private static Thread readThread;
	
	//network variables
	private Socket sock;
	private DataInputStream input;
	private DataOutputStream output;
	private static Boolean active;
	
	//GUI variables
	private static JLabel logs_label = new JLabel("Chat Logs");
	private static JTextArea chatLogs_field = new JTextArea();
	private static JLabel username_label = new JLabel("Username");
	private static JTextField username_field = new JTextField();
	private static JButton connect_button = new JButton("Connect");
	private static JButton disconnect_button = new JButton("Disconnect");
	private static JTextField sendMessage_field = new JTextField();
	private static JButton sendMsg_button = new JButton("Send");
	
	/**Sets up client**/
	public Client(String u, String h, Integer p) {
		
		try {
			
			sock = new Socket(h, p);
			input = new DataInputStream(new BufferedInputStream(sock.getInputStream()));
			output = new DataOutputStream(new BufferedOutputStream(sock.getOutputStream()));
			active = true;
			sendData("CONNECTED|" + u);
			readThread = new Thread(this);
			readThread.start();
			
		} catch (Exception e) {
			System.out.println("Exception caught: " + e);
		}
		
	}
	
	/**main reading thread**/
	public void run() {
		
		while (active) {
			
			try {
				
				String in = input.readUTF();
				if (in.equals(USER + " has disconnected.")) {
					System.out.println("Close");
				}
				chatLogs_field.append(in + "\n");
				
			} catch (Exception e) {
				System.out.println("Exception caught when reading: " + e);
			}
			
		}
		
	}
	
	/**Sends data to server**/
	public void sendData(String msg) {
		
		try {
			
			output.writeUTF(msg);
			output.flush();
			
		} catch (Exception e) {
			System.out.println("Exception caught when sending '" + msg + "': " + e);
		}
		
	}
	
	/**Stops the client networking from running**/
	public void stop() {
		
		if (readThread != null) {
			readThread.interrupt();
			readThread = null;
		}
		try {
			if (input != null) input.close();
			if (output != null) output.close();
			if (sock != null) sock.close();
		} catch (Exception e) {
			
		}
		
	}
	
	/**Loads GUI**/
	private static void loadInterface() {
		
		/**Setup window**/
		JFrame frame = new JFrame("Chat Program");
		frame.setSize(500, 450);
		frame.addWindowListener(new WindowAdapter() {
			
			public void windowClosing(WindowEvent e) {
				
				if (c != null) {
					c.sendData("DISCONNECTED|" + USER);
				}
				System.exit(0);
				
			}
			
		});
		
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
		username_field.setText("");
		content.add(username_field);
		
		connect_button.addActionListener(new ActionListener() {
			
			public void actionPerformed(ActionEvent e) {
				
				String username = username_field.getText();
				username_field.setText("");
				
				if (username != "" && username != null) {
					
					/**establish connection for username**/
                	try {
                		
                		USER = username;
                		c = new Client(USER, HOST, PORT);
                		connect_button.setEnabled(false);
                		disconnect_button.setEnabled(true);
                		sendMsg_button.setEnabled(true);
                		
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
            	
                active = false;
                connect_button.setEnabled(true);
                disconnect_button.setEnabled(false);
                sendMsg_button.setEnabled(false);
                c.sendData("DISCONNECTED|" + USER);
                
            }
            
        });
        content.add(disconnect_button);
        
        sendMessage_field.setPreferredSize(new Dimension(200, 20));
        sendMessage_field.setText("");
        content.add(sendMessage_field);
        
        sendMsg_button.setPreferredSize(new Dimension(75, 30));
        sendMsg_button.setEnabled(false);
        sendMsg_button.addActionListener(new ActionListener() {
        	
        	public void actionPerformed(ActionEvent e) {
        		
        		String toSend = sendMessage_field.getText();
        		sendMessage_field.setText("");
        		if (toSend != "") {
        			
        			c.sendData("SEND|" + toSend);
        			
        		}
        		
        	}
        	
        });
        content.add(sendMsg_button);
        
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
        sl.putConstraint(SpringLayout.NORTH, sendMsg_button, 10, SpringLayout.SOUTH, chatLogs_field);
        sl.putConstraint(SpringLayout.WEST, sendMsg_button, 5, SpringLayout.EAST, sendMessage_field);
        
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