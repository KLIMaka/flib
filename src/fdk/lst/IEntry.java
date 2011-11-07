package fdk.lst;

import java.io.BufferedWriter;
import java.io.IOException;

public interface IEntry {
    public abstract String getValue();

    public abstract int getIndex();

    public abstract void write(BufferedWriter bw) throws IOException;
}
