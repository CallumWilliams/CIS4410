import java.net.*;
import java.io.*;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class Server {
	
	private ArrayList<ServerUser> users = new ArrayList<ServerUser> ();
	private ServerSocket serv;
	private Socket sock;
	private DataInputStream input;
	private DataOutputStream output;
	
	public Server(int port) {
		
		try {
			
			serv = new ServerSocket(port);
			System.out.println("Server started.");
			
			while (true) {
				
				sock = serv.accept();
				input = new DataInputStream(new BufferedInputStream(sock.getInputStream()));
				output = new DataOutputStream(new BufferedOutputStream(sock.getOutputStream()));
				System.out.println(sock.getPort());
				readData();
				
			}
			
		} catch (Exception e) {
			System.out.println("Exception caught: " + e);
		}
		
	}
	
	public void readData() {
		
		while(true) {
			
			try {
				
				String in = input.readUTF();
				StringTokenizer tok = new StringTokenizer(in, "|");
				
				String type = tok.nextToken();
				System.out.println(in);
				if (type.equals("CONNECTED")) {
					
					//create user
					String usr = tok.nextToken();
					users.add(new ServerUser(usr, sock.getPort()));
					writeData(usr + " has connected.");
					
				} else if (type.equals("SEND")) {
					
					ServerUser s = findUser(sock.getPort());
					writeData(s.getUsername() + ": " + tok.nextToken());
					
				} else {
					System.out.println(type);
				}
				
			} catch (Exception e) {
				System.out.println("Exception caught when reading: " + e);
				System.out.println("Shutting down.");//extreme measure. Temporary.
				System.exit(0);
			}
			
		}
		
	}
	
	public void writeData(String msg) {
		
		try {
			
			output.writeUTF(msg);
			output.flush();
			
		} catch (Exception e) {
			System.out.println("Exception caught when sending '" + msg + "': " + e);
		}
		
	}
	
	public ServerUser findUser(int p) {
		
		for (int i = 0; i < users.size(); i++) {
			
			ServerUser s = users.get(i);
			if (s.getPort() == p) {
				return s;
			}
			
		}
		
		return null;
		
	}
	
	public static void main(String args[]) {
		
		if (args.length != 1) {
			System.out.println("Run as 'java Server <port>'");
		} else {
			Server server = new Server(Integer.parseInt(args[0]));
		}
		
	}
	
}