echo "Recuperation du depot de mjpg streamer."

git clone https://github.com/jacksonliam/mjpg-streamer.git
cd mjpg-streamer/mjpg-streamer-experimental/
make
sudo make install

echo "Commande de lancement de l'utilitaire. Se placer dans le dossier OrboxPFE/mjpg-streamer/mjpg-streamer-experimental."
echo ./mjpg_streamer -i "input_uvc.so $camera_usb_options" -o "output_http.so -w ./www" where camera_usb_options="-n -r VGA -f 5 -q 50"


