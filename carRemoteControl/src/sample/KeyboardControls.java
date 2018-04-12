package sample;


import javax.swing.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

public class KeyboardControls extends JFrame implements KeyListener {

    public void init(){
        addKeyListener(this);
    }

    Controller control = new Controller();
    @Override
    public void keyTyped(KeyEvent e) {

    }

    @Override
    public void keyPressed(KeyEvent event) {
        switch (event.getKeyCode()) {
            case KeyEvent.VK_UP:
                System.out.println("UP works from keyboard");
                control.forwardPressed();
                break;
            case KeyEvent.VK_LEFT:
                System.out.println("LEFT works from keyboard");
                control.leftPressed();
                break;
            case KeyEvent.VK_RIGHT:
                System.out.println("RIGHT works from keyboard");
                control.rightPressed();
                break;
            case KeyEvent.VK_DOWN:
                System.out.println("DOWN works from keyboard");
                control.reversePressed();
                break;
            default:
                System.out.println("Invalid command.");

        }


    }

    @Override
    public void keyReleased(KeyEvent e) {
        System.out.println("key is released.");
    }
}
