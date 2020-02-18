package app;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.util.Pair;

import java.util.ArrayList;


public class Game {
    private volatile int round;
    private volatile char currentLetter;
    private String clientNick;
    private ObservableList<Player> players;


    public Game(String playerNick){
        this.clientNick = playerNick;
        this.round = -1250;
        this.currentLetter = ' ';
        this.players = FXCollections.observableArrayList();
    }

    public void resolve(String input){
        // because of TCP sending everything at once we have to split messages if they come at once
        var msgArray = input.split("\n+");

        // assuming message looks like this:
        // xxxxxx::actual-message
        // where xxxxxx is round number

        for (String msg: msgArray)
        {
            String roundString = msg.substring(0, 6);
            String message = msg.substring(8);

            // Starting new round
            try{
                if(message.substring(0, 15).equals("Starting round:")){
                    this.round = Integer.parseInt(message.substring(16).trim());
                }

                else if(message.substring(0, 15).equals("New letter is :")){
                    currentLetter = message.charAt(16);
                }

                else if(message.substring(0, 5).equals("Nick:")){
                    updatePlayerState(message);
                }
            }
            catch(StringIndexOutOfBoundsException ignored){}
        }
    }

    private void updatePlayerState(String message){
        var list = message.split(";");
        var nick = getNickFromMessage(list[0]);
        var answers = new ArrayList<String>();
        var points = 0;

        for(int i = 1; i < list.length; i++){
            try{
                if(list[i].trim().substring(0, 6).equals("Answer")){
                    var pair = getAnswerAndPoints(list[i]);
                    points += pair.getValue();
                    answers.add(pair.getKey());
                }
            }
            catch(StringIndexOutOfBoundsException ignored){}
        }

        boolean found = false;
        for (Player p: getPlayers()) {
            if(p.getNick().equals(nick) && answers.size() >= 4)
            {
                p.setData(answers);
                p.setPoints(p.getPoints() + points);
                found = true;
                break;
            }
        }

        if(!found && answers.size() >= 4)
            this.getPlayers().add(new Player
                            (nick,
                             answers.get(0),
                             answers.get(1),
                             answers.get(2),
                             answers.get(3),
                             points));
    }

    private String getNickFromMessage(String msg){
        var list = msg.split(" ");
        var nick = "";
        if(list[0].trim().equals("Nick:")){
            for(int i = 1; i < list.length; i++){
                nick = nick.concat(list[i]) + " ";
            }
        }
        return nick.trim();
    }

    private Pair<String, Double> getAnswerAndPoints(String msg){
        var list = msg.trim().split(" ");
        var answer = list[1].trim();
        var points = Double.parseDouble(list[3].trim());

        return new Pair<>(answer, points);
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
