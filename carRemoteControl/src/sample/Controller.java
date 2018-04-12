package sample;


import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;




public class Controller {

    public ImageView forward;
    public ImageView right;
    public ImageView left;
    public ImageView reverse;
    public Button startV2V;
    public Button stopV2V;
    public Label stopV2VMessageSuccess;
    public Label stopV2VMessageFail;


    Image forwardButtonPressed = new Image("sample/forwardImgPressed.png");
    Image forwardButtonReleased = new Image("sample/forwardImg.png");

    Image rightButtonPressed = new Image("sample/rightImgPressed.png");
    Image rightButtonReleased = new Image("sample/rightImg.png");

    Image leftButtonPressed = new Image("sample/leftImgPressed.png");
    Image leftButtonReleased = new Image("sample/leftImg.png");

    Image reverseButtonPressed = new Image("sample/reverseImgPressed.png");
    Image reverseButtonReleased = new Image("sample/reverseImg.png");



    //Forward button:
    public boolean forwardPressed() {

        forward.setImage(forwardButtonPressed);

        System.out.println("UP WORKS!");
    return true;
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
        if (disconnectV2V() == true){
            stopV2VMessageSuccess.setTextFill(Color.GREEN);
            stopV2VMessageSuccess.setVisible(true);
        }
        else if (disconnectV2V() == false) {
            stopV2VMessageFail.setTextFill(Color.INDIANRED);
            stopV2VMessageFail.setVisible(true);
        }
    }

    public boolean disconnectV2V(){
        return false;

    }



}
