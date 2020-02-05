package app;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;

public class Connection {
    private String ip;
    private int port;
    private SocketChannel channel;

    public int getPort() {
        return port;
    }

    public String getIp() {
        return ip;
    }

    public Connection(String IP, int port) throws IOException {
        this.port = port;
        this.ip = IP;
        this.connect();
    }

    private void connect() throws IOException {
        var address = new InetSocketAddress(ip, port);
        this.channel = SocketChannel.open(address);
        System.out.println("Established new connection with address " + this.ip + " on socket " + this.port);
    }

    public void send(String msg) throws IOException {
        var buffer = ByteBuffer.wrap(msg.getBytes());
        channel.write(buffer);
        buffer.clear();
    }

    public String read() throws IOException {
        var buffer = ByteBuffer.allocate(512);
        int len = channel.read(buffer);
        byte[] bytes = new byte[len];

        for(int i = 0; i < len; i++){
            byte f = buffer.get(i);
            bytes[i] = f;
        }

        return new String(bytes, Charset.defaultCharset());
    }
}
