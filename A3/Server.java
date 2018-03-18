import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class Server implements Runnable {
	
	private ArrayList<ServerUser> users = new ArrayList<ServerUser> ();
	private Thread thread;
	private ServerSocket serv;
	private Socket sock;
	private DataInputStream input;
	
	public Server(int port) {
		
		try {
			
			serv = new ServerSocket(port);
			System.out.println("Server started.");
			
			while (true) {
				
				sock = serv.accept();
				System.out.println(sock);
				users.add(new ServerUser(sock));
				System.out.println("added " + sock.getPort() + " now at " + users.size());
				input = new DataInputStream(new BufferedInputStream(sock.getInputStream()));
				thread = new Thread(this);
				thread.start();
				
			}
			
		} catch (Exception e) {
			System.out.println("Exception caught: " + e);
		}
		
	}
	
	/**Main listening method**/
	public void run () {
		
		while (true) {
			
			try {
				
				String in = input.readUTF();
				System.out.println(in + " received from " + sock.getPort());
				StringTokenizer tok = new StringTokenizer(in, "|");
				String type = tok.nextToken();
				
				if (type.equals("CONNECTED")) {
					
					//create user
					String usr = tok.nextToken();
					int loc = findUser(sock.getPort());
					if (loc == -1) {
						System.out.println(usr + " not found in list.");
					} else {
						users.get(loc).setUsername(usr);
						writeData(usr + " has connected.");
					}
					
				} else if (type.equals("DISCONNECTED")) {
					
					//find user
					int loc = findUser(sock.getPort());
					ServerUser s = users.get(loc);
					
					//remove user
					writeData(s.getUsername() + " has disconnected.");
					users.remove(loc);
					
				} else if (type.equals("SEND")) {
					
					String usr = tok.nextToken();
					writeData(usr + ": " + tok.nextToken());
					
				} else {
					System.out.println("Error " + type);
				}
				
			} catch (Exception e) {
				System.out.println("Exception caught when reading: " + e);
				System.out.println("Shutting down.");//extreme measure. Temporary.
				System.exit(0);
			}
			
		}
		
	}
	
	public void writeData(String msg) {
		
		/**Calls all existing users' writeData() methods**/
		for (int i = 0; i < users.size(); i++) {
			
			users.get(i).writeData(msg);
			
		}
	}
	
	/**Search through connected users and find the one on port p**/
	public int findUser(int port) {
		
		for (int i = 0; i < users.size(); i++) {
			
			ServerUser s = users.get(i);
			if (s.getPort() == port) {
				return i;
			}
			
		}
		
		return -1;
		
	}
	
	public static void main(String args[]) {
		
		if (args.length != 1) {
			System.out.println("Run as 'java Server <port>'");
		} else {
			Server server = new Server(Integer.parseInt(args[0]));
		}
		
	}
	
}