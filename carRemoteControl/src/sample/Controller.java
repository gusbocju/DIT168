package sample;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;

import javax.swing.*;
import java.awt.*;


public class Controller {

    public ImageView forward;
    public ImageView right;
    public ImageView left;
    public ImageView reverse;
    public ImageView stop;

    Image forwardButtonPressed = new Image("sample/forwardImgPressed.png");
    Image forwardButtonReleased = new Image("sample/forwardImg.png");

    Image rightButtonPressed = new Image("sample/rightImgPressed.png");
    Image rightButtonReleased = new Image("sample/rightImg.png");

    Image leftButtonPressed = new Image("sample/leftImgPressed.png");
    Image leftButtonReleased = new Image("sample/leftImg.png");

    Image reverseButtonPressed = new Image("sample/reverseImgPressed.png");
    Image reverseButtonReleased = new Image("sample/reverseImg.png");

    Image stopButton = new Image("sample/stopButton.png");


    public native boolean press(char i);

    //Forward button:
    public void forwardPressed() {
        forward.setImage(forwardButtonPressed);
    }
    public void forwardRelease() {
        forward.setImage(forwardButtonReleased);
    }

    //Right button:
    public void rightPressed() {
        right.setImage(rightButtonPressed);
    }

    public void rightRelease() {
        right.setImage(rightButtonReleased);
    }

    //Left button:
    public void leftPressed() {
        left.setImage(leftButtonPressed);
    }

    public void leftRelease() {
        left.setImage(leftButtonReleased);
    }

    //Reverse button:
    public void reversePressed() {
        reverse.setImage(reverseButtonPressed);
    }

    public void reverseRelease() {
        reverse.setImage(reverseButtonReleased);
    }

    //Stop Car button:
    public void stopCar() {
        stop.setImage(stopButton);
    }

    public void notStop() {
        stop.setImage(stopButton);

    }
}
