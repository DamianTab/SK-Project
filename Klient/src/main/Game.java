package main;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;


public class Game {
    private volatile int round;
    private char currentLetter;
    private String clientNick;
    private ObservableList<Player> players;


    public Game(String playerNick){
        this.clientNick = playerNick;
        this.round = 0;
        this.currentLetter = ' ';
        this.players = FXCollections.observableArrayList();
    }

    public void resolve(String msg){
        // assuming message looks like this:
        // xxxxxx::actual-message
        // where xxxxxx is round number

        String roundString = msg.substring(0, 6);
        String message = msg.substring(8);

        // Starting new round
        if(message.substring(0, 15).equals("Starting round:")){
            this.round = Integer.parseInt(message.substring(16).trim());
        }

        else if(message.substring(0, 15).equals("New letter is :")){
            currentLetter = message.charAt(16);
        }

    }

    public String getClientNick() {
        return clientNick;
    }

    public int getRound() {
        return round;
    }

    public ObservableList<Player> getPlayers() {
        return players;
    }

    public char getCurrentLetter() {
        return currentLetter;
    }

    public void setCurrentLetter(char currentLetter) {
        this.currentLetter = currentLetter;
    }
}
