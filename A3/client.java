import javax.swing.SpringLayout;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class client extends JFrame {
    
    private static void loadInterface() {
        
        /**Setup window**/
        JFrame frame = new JFrame("Client");
        frame.setSize(500, 450);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        /**Setup content pane**/
        Container content = frame.getContentPane();
        SpringLayout sl = new SpringLayout();
        content.setLayout(sl);
        
        /**Setup elements**/
        JLabel logs_label = new JLabel("Chat Logs");
        content.add(logs_label);
        
        JTextArea chatLogs_field = new JTextArea();
        chatLogs_field.setPreferredSize(new Dimension(250, 375));
        chatLogs_field.setEditable(false);
        content.add(chatLogs_field);
        
        JLabel username_label = new JLabel("Username");
        content.add(username_label);
        
        JTextField username_field = new JTextField();
        username_field.setPreferredSize(new Dimension(150, 20));
        content.add(username_field);
        
        JButton connect_button = new JButton("Connect");
        connect_button.addActionListener(new ActionListener() {
            
            public void actionPerformed(ActionEvent e) {
                System.out.println("connect");
            }
            
        });
        content.add(connect_button);
        
        JButton disconnect_button = new JButton("Disconnect");
        
        disconnect_button.addActionListener(new ActionListener() {
            
            public void actionPerformed(ActionEvent e) {
                System.out.println("disconnect");
            }
            
        });
        content.add(disconnect_button);
        
        JTextField sendMessage_field = new JTextField();
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
        
        loadInterface();
        
    }
    
}