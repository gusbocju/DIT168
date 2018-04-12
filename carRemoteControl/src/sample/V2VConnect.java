package sample;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class V2VConnect {


    public Button connect;
    public TextField carIdLabel;
    public Label errorMessage;


    public void v2VWindow(){
    try {
        FXMLLoader fxmlLoader = new FXMLLoader();
        fxmlLoader.setLocation(getClass().getResource("popUp.fxml"));

        Scene scene = new Scene(fxmlLoader.load(), 300, 200);
        Stage stage = new Stage();
        stage.setTitle("V2VConnect");
        stage.setScene(scene);
        stage.show();
    }
    catch (IOException e) {
        Logger logger = Logger.getLogger(getClass().getName());
        logger.log(Level.SEVERE, "Failed to create new Window.", e);
    }
    }



    @FXML
    public void windowAction() {
        if (v2VConnectionSuccess() == true){
            Stage stage = (Stage) connect.getScene().getWindow();
            stage.close();
        }
        else {
            errorMessage.setTextFill(Color.INDIANRED);
            errorMessage.setVisible(true);
        }

    }

    private boolean v2VConnectionSuccess(){
        return false;
    }



}


