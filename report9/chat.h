#ifndef CHAT_H
#define CHAT_H

#define MAX_TEXT 512

struct message {
    long msg_type;
    char msg_text[MAX_TEXT];
};

#endif
