package sample;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.image.ImageView;
import javafx.scene.image.Image;
import javax.imageio.ImageIO;
import javafx.event.*;
import java.awt.*;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        Parent root = FXMLLoader.load(getClass().getResource("sample.fxml"));
        primaryStage.setTitle("Remote Control");
        primaryStage.setScene(new Scene(root, 400, 450));
        primaryStage.show();

    }



    public static void main(String[] args) {
        launch(args);
    }
}
