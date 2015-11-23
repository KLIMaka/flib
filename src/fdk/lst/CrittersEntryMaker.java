package fdk.lst;

import java.io.*;

public class CrittersEntryMaker implements IEntryMaker {

	static public class Entry extends BasicEntryMaker.Entry {

		public Entry(String val, int idx) {
			super(val, idx);
		}

		@Override
		public String getValue() {
			return getContent().replaceFirst(",.*", "");
		}

	}

	@Override
	public IEntry create(int index, BufferedReader reader) throws IOException {
		if (!reader.ready())
			return null;
		Entry ent = new Entry(reader.readLine(), index);
		return ent;
	}

}
