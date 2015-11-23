package fdk.util;

import java.io.*;

public class Utils {

	public static int toLittleEndian(int data) {
		return Integer.reverseBytes(data) /* & 0xffffffff */;
	}

	public static int copyStrream(InputStream in, OutputStream out) throws IOException {
		int copied = 0;
		byte buffer[] = new byte[1024];
		int len = 0;

		while ((len = in.read(buffer)) != -1) {
			out.write(buffer, 0, len);
			copied += len;
		}

		return copied;
	}
}
