package main;

// GUI imports
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextInputDialog;
import javafx.stage.Stage;
import javax.swing.*;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        var loader = new FXMLLoader(getClass().getResource("scene.fxml"));

        Parent root = loader.load();

        var dialog = new TextInputDialog();
        var parameters = getParameters().getRaw();

        dialog.setHeaderText(null);
        dialog.setContentText("Wprowadź swój nick:");

        var optionalNick = dialog.showAndWait();

        String nick = "";
        if(optionalNick.isPresent()){
            nick = optionalNick.get();
        }

        String IP = "127.0.0.1";
        int port = 5000;

        if(parameters.size() >= 2){
            IP = parameters.get(0);
            port = Integer.parseInt(parameters.get(1));
        }


        Controller controller = loader.getController();
        controller.setup(nick);

        try{

            controller.setConnection(new Connection(IP, port));
            controller.getConnection().send("0000-1::" + nick + " ");
            // right here thread starts to accept messages
            new Thread(controller).start();
        }
        catch(Exception e){
            System.out.println("Could not establish connection with the server at address " + IP + " on port " + port);
        }

        primaryStage.setTitle("Państwa - miasta");
        primaryStage.setScene(new Scene(root, 900, 500));
        primaryStage.show();
    }


    public static void main(String[] args)
    {
        launch(args);
    }
}
