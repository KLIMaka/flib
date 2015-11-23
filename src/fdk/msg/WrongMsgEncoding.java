package fdk.msg;

import java.io.IOException;

@SuppressWarnings("serial")
public class WrongMsgEncoding extends IOException {

	public WrongMsgEncoding(String msg) {
		super(msg);
	}
}
