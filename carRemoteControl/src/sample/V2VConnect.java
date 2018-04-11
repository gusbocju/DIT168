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

    public void v2VWindow(){
    try {
        FXMLLoader fxmlLoader = new FXMLLoader();
        fxmlLoader.setLocation(getClass().getResource("popUp.fxml"));
        /*
         * if "fx:controller" is not set in fxml
         * fxmlLoader.setController(NewWindowController);
         */

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

    //FIXME: empty method duplicate. FIX what is under.
    public void windowAction(ActionEvent actionEvent) {
    }

    /*
    @FXML
    private javafx.scene.control.Button closeButton;
    public void windowAction() {
        if (v2VConnectionSuccess() == true){
            Stage stage = (Stage) closeButton.getScene().getWindow();
            stage.close();
        }
        else {
            // FIXME: FIX ERROR MESSAGE
            Label message = new Label();
            message.setText("Connection Failed!");
            message.setTextFill(Color.rgb(210, 39, 30));

        }

    }

    private boolean v2VConnectionSuccess(){
        return true;
    }

    */



}


