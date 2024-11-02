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
docker exec -w"/root/Toolchain" -it $nombre make clean all
docker exec -w"/root" -it $nombre make clean all
docker exec -w"/root" -it $nombre chmod 777 /root/Image/x64BareBonesImage.qcow2
docker stop $nombre &> /dev/null
exit 0