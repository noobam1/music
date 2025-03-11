import yt_dlp
import os
from telegram import Update
from telegram.ext import Updater, CommandHandler, CallbackContext

# Replace with your Bot's token
YOUR_BOT_TOKEN = '6824476193:AAE4KgvS24oCNCfChNvo90uRUgvH7wAQ6ZY'

# Replace with your admin/supporter ID (the person who streams music)
SUPPORTER_ID = 5859478478  # Replace with the second user's Telegram ID
ADMIN_ID = 708030615  # Replace with your admin ID (person who can send broadcast messages)

# This will store the user IDs of everyone who interacts with the bot
user_ids = set()  # A set is used to prevent duplicate entries

# Function to search and download the song by name
def download_song(song_name: str):
    ydl_opts = {
        'format': 'bestaudio/best',
        'noplaylist': True,
        'outtmpl': 'downloads/%(title)s.%(ext)s',  # Save in 'downloads' directory
        'postprocessors': [{
            'key': 'FFmpegAudioConvertor',
            'preferredcodec': 'mp3',
            'preferredquality': '192',
        }],
    }

    # Search the song by name using yt-dlp
    with yt_dlp.YoutubeDL(ydl_opts) as ydl:
        search_query = f"ytsearch:{song_name}"
        info_dict = ydl.extract_info(search_query, download=True)
        # Return the file path of the downloaded audio
        return os.path.join('downloads', f"{info_dict['title']}.mp3")

# Function to start the bot
def start(update: Update, context: CallbackContext) -> None:
    user_id = update.message.from_user.id
    user_ids.add(user_id)  # Add the user's ID to the user list
    update.message.reply_text('Hello! I am your music bot. Type /play <song_name> to play music.')

# Function to play music
def play_music(update: Update, context: CallbackContext):
    try:
        song_name = " ".join(context.args)  # Get the song name from the command
        if not song_name:
            update.message.reply_text("Please provide a song name to play.")
            return
        
        # Download the song
        song_file = download_song(song_name)
        
        # Notify the second user (supporter) to start streaming
        context.bot.send_message(chat_id=SUPPORTER_ID, text=f"Start streaming the song: {song_name}. File path: {song_file}")
        
        # Inform the user
        update.message.reply_text(f"Song '{song_name}' is being prepared for streaming!")

    except Exception as e:
        update.message.reply_text(f"Error: {e}")

# Function for the supporter to start streaming the music (handled separately)
def stream_music():
    # This should be done manually by the second user (supporter)
    # The supporter will handle FFmpeg streaming
    pass

# Function to join the voice chat (placeholder for supporter)
def join_voice_chat(update: Update, context: CallbackContext):
    chat = update.message.chat
    update.message.reply_text("Joining voice chat... (This part is handled by the second user)")

# Function for the admin to broadcast messages to all users
def broadcast(update: Update, context: CallbackContext):
    if update.message.from_user.id != ADMIN_ID:  # Check if the user is the admin
        update.message.reply_text("You don't have permission to send broadcast messages.")
        return

    # Get the message that the admin wants to broadcast
    message = " ".join(context.args)  # The message after the command
    if not message:
        update.message.reply_text("Please provide a message to broadcast.")
        return

    # Send the message to all stored user IDs
    for user_id in user_ids:
        try:
            context.bot.send_message(chat_id=user_id, text=message)
        except Exception as e:
            print(f"Could not send message to {user_id}: {e}")

    # Only send a confirmation to the admin
    update.message.reply_text(f"Broadcast message sent to {len(user_ids)} users.")

# Main function to set up the bot
def main():
    updater = Updater(YOUR_BOT_TOKEN)
    dispatcher = updater.dispatcher

    # Command handlers
    dispatcher.add_handler(CommandHandler("start", start))
    dispatcher.add_handler(CommandHandler("play", play_music))
    dispatcher.add_handler(CommandHandler("join", join_voice_chat))
    dispatcher.add_handler(CommandHandler("broadcast", broadcast))

    # Start the bot
    updater.start_polling()
    updater.idle()

if __name__ == '__main__':
    main()
