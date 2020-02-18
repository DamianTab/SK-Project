package app;

import javafx.application.Platform;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Controller implements Runnable
{
    private Connection connection;
    private Game game;
    private int lastRoundSent;

    //Labels
    public Label nickLabel;
    public Label roundLabel;
    public Label letterLabel;

    //TextFields
    public TextField countryField;
    public TextField cityField;
    public TextField animalField;
    public TextField nameField;

    // Table container variables
    public TableView<Player> table;
    public TableColumn<Player, String> nickColumn;
    public TableColumn<Player, String> countryColumn;
    public TableColumn<Player, String> cityColumn;
    public TableColumn<Player, String> animalColumn;
    public TableColumn<Player, String> nameColumn;
    public TableColumn<Player, Integer> pointsColumn;

    public Button button;

    @FXML
    private void initialize(){
        nickColumn.setCellValueFactory(new PropertyValueFactory<>("nick"));
        countryColumn.setCellValueFactory(new PropertyValueFactory<>("country"));
        cityColumn.setCellValueFactory(new PropertyValueFactory<>("city"));
        animalColumn.setCellValueFactory(new PropertyValueFactory<>("animal"));
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("name"));
        pointsColumn.setCellValueFactory(new PropertyValueFactory<>("points"));

        table.setPlaceholder(new Label("Wyniki rundy pojawią się po jej zakończeniu"));

        button.setDisable(true);

        button.setOnMouseClicked(mouseEvent -> {
            this.onClick();
        });
    }

    public void setup(String nickName){
        game = new Game(nickName);
        nickLabel.setText(game.getClientNick());
        table.setItems(game.getPlayers());
    }

    @FXML
    @SuppressWarnings("InfiniteLoopStatement")
    public void run() {
        try {
            while(true){
                String msg = connection.read();
                System.out.println(msg);
                game.resolve(msg);


                Platform.runLater(() -> {
                    if(lastRoundSent < game.getRound()){
                        button.setDisable(false);
                    }
                    roundLabel.setText("Runda: " + game.getRound());
                    letterLabel.setText("Litera: " + game.getCurrentLetter());
                });
            }
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }

    public void onClick(){

        // ensuring that we will send result only once per round
        if(game.getRound() == lastRoundSent){
            System.out.println("You have sent a message this turn!");
            return;
        }

        String country = countryField.getText();
        String city = cityField.getText();
        String animal = animalField.getText();
        String name = nameField.getText();

        var answers = new ArrayList<>(Arrays.asList(country, city, animal, name));

//        if(areAnswersValid(answers)){
            String header = game.getRound() + "::";
            String fill = "0".repeat(8 - header.length());

            header = fill.concat(header);
            String message = "";
            for(String answer: answers){
                message = message.concat(answer + ", ");
            }
            message = message.substring(0, message.length() - 2);
            message = header.concat(message);

            try {
                lastRoundSent = game.getRound();
                System.out.println(message);
                button.setDisable(true);
                connection.send(message);
            } catch (IOException e) {
                System.err.println("Could not send message due to connection error");
            }
//        }
    }

    public Connection getConnection() {
        return connection;
    }


    private boolean areAnswersValid(List<String> answers){
        if(answers.size() < 1) return false;
        for(String ans: answers){
            if(!ans.toLowerCase().startsWith(game.getCurrentLetter() + "")) return false;
        }
        return true;
    }

    public void setConnection(Connection connection) {
        this.connection = connection;
    }
}
