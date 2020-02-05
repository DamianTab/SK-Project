package app;

import javafx.beans.property.*;

import java.util.ArrayList;

public class Player {
    private SimpleStringProperty nick;
    private SimpleStringProperty country;
    private SimpleStringProperty city;
    private SimpleStringProperty animal;
    private SimpleStringProperty name;
    private SimpleDoubleProperty points;
    private int roundsInactive;

    public Player(String nick,
                  String country,
                  String city,
                  String animal,
                  String name,
                  double points) {
        this.nick = new SimpleStringProperty(nick);
        this.country = new SimpleStringProperty(country);
        this.city = new SimpleStringProperty(city);
        this.animal = new SimpleStringProperty(animal);
        this.name = new SimpleStringProperty(name);
        this.points = new SimpleDoubleProperty(points);
        this.roundsInactive = 0;
    }

    public int getRoundsInactive() {
        return roundsInactive;
    }

    public void setRoundsInactive(int roundsInactive) {
        this.roundsInactive = roundsInactive;
    }

    public void setData(ArrayList<String> params){
        setCountry(params.get(0));
        setCity(params.get(1));
        setAnimal(params.get(2));
        setName(params.get(3));
    }

    public String getNick() {
        return nick.get();
    }

    public StringProperty nickProperty() {
        return nick;
    }

    public void setNick(String nick) {
        this.nick.set(nick);
    }

    public String getCountry() {
        return country.get();
    }

    public StringProperty countryProperty() {
        return country;
    }

    public void setCountry(String country) {
        this.country.set(country);
    }

    public String getCity() {
        return city.get();
    }

    public StringProperty cityProperty() {
        return city;
    }

    public void setCity(String city) {
        this.city.set(city);
    }

    public String getAnimal() {
        return animal.get();
    }

    public StringProperty animalProperty() {
        return animal;
    }

    public void setAnimal(String animal) {
        this.animal.set(animal);
    }

    public String getName() {
        return name.get();
    }

    public StringProperty nameProperty() {
        return name;
    }

    public void setName(String name) {
        this.name.set(name);
    }

    public double getPoints() {
        return points.get();
    }

    public DoubleProperty pointsProperty() {
        return points;
    }

    public void setPoints(double points) {
        this.points.set(points);
    }
}
