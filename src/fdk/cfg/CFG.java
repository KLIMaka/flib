package fdk.cfg;

import java.io.*;
import java.nio.*;
import java.nio.charset.*;
import java.util.*;
import java.util.regex.*;

import fdk.util.Utils;

/**
 * Класс для работы с CFG файлами.
 * 
 * CFG файл представляет из себя хранилище пар ключ-значение, разделенное на
 * категории.<br>
 * Пример файла:
 * 
 * <pre>
 * ;комментарий. Строка, предворяемая символом ';' становится комментарием
 * Name=Value     ; пара ключ-значение, не отнесенная ни к одному из разделов.
 *                ; имя и значение могут содержать любые символы кроме переноса строки и ';'
 * [Main]         ; имя раздела
 * Name1=Value1   ; пара, отнесенная к разделу Main. Может быть доступна как "Main.Name1"
 *                ; или "Name1" если такое имя единствено во всем файле. Регистр значения не имеет
 * ;Name2=Value2
 * </pre>
 * 
 * Файл может быть сохранен в любой кодировке. В качестве перевода строки
 * обязательно использовать <code>CRLF</code>. <br>
 * Класс позволяет сохранить комментарии
 * 
 * @author KLIMaka
 * 
 */
public class CFG {

	private Map<String, ArrayList<CFGEntry>> m_config = new TreeMap<String, ArrayList<CFGEntry>>(String.CASE_INSENSITIVE_ORDER);
	private ArrayList<String> m_catOrder = new ArrayList<String>();

	public CFG(InputStream in) throws IOException {
		read(in);
	}

	public CFG() {
	}

	public void read(InputStream in) throws FileNotFoundException, IOException {
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		Utils.copyStrream(in, out);
		ByteBuffer buf = ByteBuffer.wrap(out.toByteArray());
		CharsetDecoder cd = Charset.defaultCharset().newDecoder();
		CharBuffer cb = cd.decode(buf);
		Pattern p = Pattern
				.compile("((((;.*\\r\\n)|\\r\\n)*)(\\[(.*)\\]) *(;(.*))?\\r\\n)|((((;.*\\r\\n)|\\r\\n)*?)(([^\\r\\n]*)=([^;\\n\\r]*))(;(.*)(\\r\\n)?)?)");
		Matcher m = p.matcher(cb);

		String category = "";
		while (m.find()) {
			if (m.group(1) != null) {
				category = m.group(6);
				String precomment = m.group(2);
				String postcomment = m.group(8);
				getCategory(category).add(new CFGEntry(category, "@head", "", precomment, postcomment, false));
			} else {
				String precomment = m.group(10);
				String name = m.group(14).trim();
				String val = m.group(15).trim();
				String postcomment = m.group(17);
				boolean enabled = true;
				if (name.startsWith(";")) {
					name = name.substring(1);
					enabled = false;
				}

				getCategory(category).add(new CFGEntry(category, name, val, precomment, postcomment, enabled));
			}
		}

	}

	public void clear() {
		m_config.clear();
		m_catOrder.clear();
	}

	public Collection<CFGEntry> getCategory(String cat) {
		ArrayList<CFGEntry> category = m_config.get(cat);
		if (category == null) {
			m_catOrder.add(cat);
			category = new ArrayList<CFGEntry>();
			m_config.put(cat, category);
		}
		return category;
	}

	public CFGEntry get(String cat, String param) {
		for (CFGEntry ent : getCategory(cat)) {
			if (ent.getName().equalsIgnoreCase(param))
				return ent;
		}
		return null;
	}

	public CFGEntry get(String param) {
		String parts[] = param.split("\\.");
		if (parts.length == 2) {
			return get(parts[0], parts[1]);
		} else {
			for (Collection<CFGEntry> cat : m_config.values()) {
				for (CFGEntry ent : cat) {
					if (ent.getName().equalsIgnoreCase(param))
						return ent;
				}
			}
			return null;
		}
	}

	public CFGEntry get(String param, boolean enabled) {
		CFGEntry ent = get(param);
		if (ent.isEnabled() == enabled) {
			return ent;
		}
		return null;
	}

	public String getValue(String param) {
		CFGEntry ent = get(param, true);
		if (ent != null) {
			return ent.getValue();
		} else {
			return null;
		}
	}

	public void write(OutputStream out) throws IOException {
		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(out));
		for (String cat : m_catOrder) {
			CFGEntry head = get(cat, "@head");
			if (head.getPrecomment() != null) {
				writer.write(head.getPrecomment());
			} else {
				writer.write("\r\n");
			}
			writer.write("[" + cat + "]");
			if (head.getPostcomment() != null) {
				writer.write(" ;" + head.getPostcomment());
			}
			writer.write("\r\n");

			for (CFGEntry ent : getCategory(cat)) {
				if (ent.getName().equals("@head"))
					continue;

				if (ent.getPrecomment() != null) {
					writer.write(ent.getPrecomment());
				} else {
					writer.write("\r\n");
				}
				writer.write(ent.isEnabled() ? "" : ";");
				writer.write(ent.getName() + "=" + ent.getValue());
				if (ent.getPostcomment() != null)
					writer.write(" ;" + ent.getPostcomment() + "\r\n");
			}
		}
		writer.close();
	}

	public Collection<String> getCategories() {
		return m_catOrder;
	}

	public void put(CFGEntry ent) {
		getCategory(ent.getCategory()).add(ent);
	}
}
