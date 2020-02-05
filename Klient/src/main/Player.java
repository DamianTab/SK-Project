package main;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

public class Player {
    private SimpleStringProperty nick;
    private SimpleStringProperty country;
    private SimpleStringProperty city;
    private SimpleStringProperty animal;
    private SimpleStringProperty name;
    private SimpleIntegerProperty points;

    public Player(String nick,
                  String country,
                  String city,
                  String animal,
                  String name,
                  int points) {
        this.nick = new SimpleStringProperty(nick);
        this.country = new SimpleStringProperty(country);
        this.city = new SimpleStringProperty(city);
        this.animal = new SimpleStringProperty(animal);
        this.name = new SimpleStringProperty(name);
        this.points = new SimpleIntegerProperty(points);
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

    public int getPoints() {
        return points.get();
    }

    public IntegerProperty pointsProperty() {
        return points;
    }

    public void setPoints(int points) {
        this.points.set(points);
    }
}
