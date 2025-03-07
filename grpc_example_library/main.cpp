#include <string>
#include <iostream>
#include <stdio.h>
#include <tgbot/tgbot.h>

#include "client.hpp"

int main() {

    //ToxicityClassifierClient client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

    TgBot::Bot bot("7229787403:AAH0DVCx0wUQ-G9lkXYoIllHL0DhmdawEZo");
    std::cout<<"input\n";
    
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {

        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });

    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        std::cout<<"input2\n";
        //float toxicity = client.ClassifyMessage(message->text);

        std::cout<<"input2\n";



        bot.getApi().sendMessage(message->chat->id, "Your message is: ");
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    std::string message = "Hello, how are you?";

    //float toxicity = client.ClassifyMessage(message);


    //std::cout << "Toxicity probability: " << toxicity << std::endl;

    return 0;
}
