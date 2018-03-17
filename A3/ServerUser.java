import java.io.*;

public class ServerUser {
	
	private String username;
	private int port;
	
	public ServerUser(String u, int p) {
		
		this.username = u;
		this.port = p;
		
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
	
}