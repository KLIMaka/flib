package fdk.msg;

public class MsgEntry {

    private int    m_number;
    private String m_sound;
    private String m_msg;
    private String m_preComment;
    private String m_postComment;

    public MsgEntry(int number, String anim, String msg, String pre, String post) {
        m_number = number;
        m_sound = anim;
        m_msg = msg;
        m_preComment = pre;
        m_postComment = post;
    }

    public int getNumber() {
        return m_number;
    }

    public String getSound() {
        if (m_sound != null) return m_sound;
        return "";
    }

    public String getMsg() {
        return m_msg;
    }

    public String getPreComment() {
        if (m_preComment != null) return m_preComment;
        return "";
    }

    public String getPostComment() {
        if (m_postComment != null) return m_postComment;
        return "";
    }

    @Override
    public String toString() {
        return getPreComment() + "{" + getNumber() + "}{" + getSound() + "}{" + getMsg() + "}" + getPostComment();
    }

    public void setMsg(String msg) {
        m_msg = msg;
    }
}
