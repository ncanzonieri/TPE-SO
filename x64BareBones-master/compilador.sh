if [ $# -ne 1 ]
then
    echo 'ingrese contenedor'
    exit 2
fi
nombre=$1
docker start $nombre &> /dev/null
if [ $? -ne 0 ]
then
    echo 'nombre no válido'
    exit 1
fi
docker exec -it $nombre make clean –C /root/Toolchain
docker exec -it $nombre make clean –C /root/
docker exec -it $nombre make –C /root/Toolchain
docker exec -it $nombre make –C /root/
docker stop $nombre &> /dev/null
exit 0