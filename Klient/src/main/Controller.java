package main;

import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;

import java.io.IOException;

public class Controller implements Runnable
{
    private Connection connection;
    private Game game;

    //Labels

    public Label nickLabel;
    public Label roundLabel;
    public Label letterLabel;

    // Table container variables
    public TableView<Player> table;
    public TableColumn<Player, String> nickColumn;
    public TableColumn<Player, String> countryColumn;
    public TableColumn<Player, String> cityColumn;
    public TableColumn<Player, String> animalColumn;
    public TableColumn<Player, String> nameColumn;
    public TableColumn<Player, Integer> pointsColumn;


    @FXML
    private void initialize(){
        nickColumn.setCellValueFactory(new PropertyValueFactory<>("nick"));
        countryColumn.setCellValueFactory(new PropertyValueFactory<>("country"));
        cityColumn.setCellValueFactory(new PropertyValueFactory<>("city"));
        animalColumn.setCellValueFactory(new PropertyValueFactory<>("animal"));
        nameColumn.setCellValueFactory(new PropertyValueFactory<>("name"));
        pointsColumn.setCellValueFactory(new PropertyValueFactory<>("points"));

        table.setPlaceholder(new Label("Wyniki rundy pojawią się po jej zakończeniu"));
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
                game.resolve(msg);

                Platform.runLater(() -> {
                    roundLabel.setText("Runda: " + game.getRound());
                    letterLabel.setText("Litera: " + game.getCurrentLetter());
                });
            }
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }

    public Connection getConnection() {
        return connection;
    }


    public void setConnection(Connection connection) {
        this.connection = connection;
    }
}
