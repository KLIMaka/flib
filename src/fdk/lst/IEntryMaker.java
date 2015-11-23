package fdk.lst;

import java.io.*;

public interface IEntryMaker {
	public abstract IEntry create(int index, BufferedReader reader) throws IOException;
}
