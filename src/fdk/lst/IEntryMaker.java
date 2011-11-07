package fdk.lst;

import java.io.BufferedReader;
import java.io.IOException;

public interface IEntryMaker {
    public abstract IEntry create(int index, BufferedReader reader) throws IOException;
}
