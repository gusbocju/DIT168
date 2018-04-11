package sample;

import javafx.application.Application;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;
import javafx.stage.Stage;


public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Remote Control");
        primaryStage.setScene(new Scene(root, 400, 450));
        primaryStage.show();


        /*
        // FIX ME
        Controller control = new Controller();
        @FXML
        public void keyPressed(K){
            switch (event.getKeyCode()) {
                case KeyEvent.VK_A:
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
                case KeyEvent:
                    System.out.println("DOWN works from keyboard");
                    control.reversePressed();
                    break;
                default:
                    System.out.println("Invalid command.");

            }

        }
    */



    }



    public static void main(String[] args) {
        launch(args);
    }
}
