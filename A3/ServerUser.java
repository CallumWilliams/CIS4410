import java.io.*;
import java.net.Socket;

public class ServerUser extends Thread {
	
	private String username;
	private Socket sock;
	private int port;
	private DataInputStream input;
	private DataOutputStream output;
	
	public ServerUser(Socket so) {
		
		this.username = null;
		this.sock = so;
		this.port = sock.getPort();
		try {
			input = new DataInputStream(new BufferedInputStream(sock.getInputStream()));
			output = new DataOutputStream(new BufferedOutputStream(sock.getOutputStream()));
		} catch (Exception e) {
			System.out.println("Exception caught when creating user " + e);
		}
		
	}
	
	/**Self-contained writing method**/
	public void writeData(String msg) {
		
		try {
			
			System.out.println(msg);
			output.writeUTF(msg);
			output.flush();
			
		} catch (Exception e) {
			System.out.println("Exception caught when sending '" + msg + "': " + e);
		}
		
	}
	
	/**Getters/Setters**/
	public void setUsername(String u) {
		this.username = u;
	}
	
	public String getUsername() {
		return this.username;
	}
	
	public void setPort(int p) {
		this.port = p;
	}
	
	public int getPort() {
		return this.port;
	}
	
}