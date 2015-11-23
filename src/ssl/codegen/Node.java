package ssl.codegen;

import java.util.Iterator;

public class Node implements Iterable<ByteStream> {

	private Node m_next = null;
	private Node m_prev = null;
	private ByteStream m_data;

	public Node(ByteStream s) {
		m_data = s;
	}

	public Node() {
		m_data = new ByteStream();
	}

	public Node getNext() {
		return m_next;
	}

	public void setNext(Node mNext) {
		m_next = mNext;
	}

	public Node getPrev() {
		return m_prev;
	}

	public void setPrev(Node mPrev) {
		m_prev = mPrev;
	}

	public Node addAfter(Node node) {
		node.getLast().setNext(getNext());
		node.getHead().setPrev(this);
		if (getNext() != null) {
			getNext().setPrev(node);
		}
		setNext(node);
		return node;
	}

	public Node addBefore(Node node) {
		node.getHead().setPrev(getPrev());
		node.getLast().setNext(this);
		if (getPrev() != null) {
			getPrev().setNext(node);
		}
		setPrev(node);
		return node;
	}

	public Node pushBack(Node node) {
		getLast().addAfter(node);
		return node;
	}

	public Node pushFront(Node node) {
		getHead().addBefore(node);
		return node;
	}

	public Node getHead() {
		if (getPrev() == null) {
			return this;
		} else {
			return getPrev().getHead();
		}
	}

	public Node getLast() {
		if (getNext() == null) {
			return this;
		} else {
			return getNext().getLast();
		}
	}

	public ByteStream get() {
		return m_data;
	}

	@Override
	public Iterator<ByteStream> iterator() {
		final Node tmp = this;
		return new Iterator<ByteStream>() {

			Node m_cur = tmp;

			@Override
			public boolean hasNext() {
				return m_cur != null;
			}

			@Override
			public ByteStream next() {
				ByteStream tmp = m_cur.m_data;
				m_cur = m_cur.getNext();
				return tmp;
			}

			@Override
			public void remove() {
			}
		};
	}
}
