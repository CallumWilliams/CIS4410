import java.io.*;
import java.net.Socket;

public class ServerUser {
	
	private String username;
	private int port;
	private DataOutputStream out;
	
	public ServerUser(String u, Socket s) {
		
		this.username = u;
		this.port = s.getPort();
		try {
			out = new DataOutputStream(new BufferedOutputStream(s.getOutputStream()));
		} catch (Exception e) {
			System.out.println("Exception caught when creating user " + u);
		}
		
	}
	
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
	
	public void setOutputStream(DataOutputStream d) {
		this.out = d;
	}
	
	public DataOutputStream getOutputStream() {
		return this.out;
	}
	
}