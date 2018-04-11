package sample;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyEvent;



public class Controller {

    public ImageView forward;
    public ImageView right;
    public ImageView left;
    public ImageView reverse;
    public Button startV2V;
    public Button stopV2V;


    Image forwardButtonPressed = new Image("sample/forwardImgPressed.png");
    Image forwardButtonReleased = new Image("sample/forwardImg.png");

    Image rightButtonPressed = new Image("sample/rightImgPressed.png");
    Image rightButtonReleased = new Image("sample/rightImg.png");

    Image leftButtonPressed = new Image("sample/leftImgPressed.png");
    Image leftButtonReleased = new Image("sample/leftImg.png");

    Image reverseButtonPressed = new Image("sample/reverseImgPressed.png");
    Image reverseButtonReleased = new Image("sample/reverseImg.png");

    /*
    FIX ME!!
    @FXML
    public void keyPressed(KeyEvent event){
        switch (event.getKeyCode()) {
            case KeyEvent.VK_A:
                System.out.println("UP works from keyboard");
                forwardPressed();
                break;
            case KeyEvent.VK_LEFT:
                System.out.println("LEFT works from keyboard");
                leftPressed();
                break;
            case KeyEvent.VK_RIGHT:
                System.out.println("RIGHT works from keyboard");
                rightPressed();
                break;
            case KeyEvent.VK_DOWN:
                System.out.println("DOWN works from keyboard");
                reversePressed();
                break;
            default:
                System.out.println("Invalid command.");

        }

    }
    */






    //Forward button:
    public void forwardPressed() {

        forward.setImage(forwardButtonPressed);

        System.out.println("UP WORKS!");
    }

    public void forwardRelease() {
        forward.setImage(forwardButtonReleased);
    }

    //Right button:
    public void rightPressed() {
        right.setImage(rightButtonPressed);
        System.out.println("RIGHT WORKS!");

    }

    public void rightRelease() {
        right.setImage(rightButtonReleased);
    }

    //Left button:
    public void leftPressed() {
        left.setImage(leftButtonPressed);
        System.out.println("LEFT WORKS!");

    }

    public void leftRelease() {
        left.setImage(leftButtonReleased);
    }

    //Reverse button:
    public void reversePressed() {
        reverse.setImage(reverseButtonPressed);
        System.out.println("REVERSE WORKS!");

    }

    public void reverseRelease() {
        reverse.setImage(reverseButtonReleased);
    }


    //Start  V2V connection
    public void startV2vPressed() {
        V2VConnect connect = new V2VConnect();
        connect.v2VWindow();

    }

    public void stopV2vPressed() {

    }

    // FIXME: this method is just a dummy. need to successfully implement keyboard controls.
    public void keyPressed(KeyEvent keyEvent) {
    }
}
