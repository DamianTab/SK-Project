package app;

// GUI imports
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.TextInputDialog;
import javafx.stage.Stage;

import static java.lang.System.exit;

public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{
        var loader = new FXMLLoader(getClass().getClassLoader().getResource("scene.fxml"));

        Parent root = loader.load();

        var dialog = new TextInputDialog();
        var parameters = getParameters().getRaw();

        dialog.setGraphic(null);
        dialog.setHeaderText(null);
        dialog.setContentText("Wprowadź swój nick:");
        dialog.setTitle("Nick");

        var optionalNick = dialog.showAndWait();

        String nick = "";
        if(optionalNick.isPresent()){
            nick = optionalNick.get();
        }
        else{
            exit(0);
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

            // right here thread starts to accept messages
            new Thread(controller).start();
            controller.getConnection().send("0000-1::" + nick + " ");
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
