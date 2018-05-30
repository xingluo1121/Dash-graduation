set terminal png truecolor size 1280,720
set output "bufferLevel.png"   
set autoscale    
set style data linespoints 
set xlabel "Time/s" 
set ylabel "Buffer/s"
set title "BufferStatus" 
set grid 
set key left
plot [0:320] [0:10] './tobasco/6/sim3_cl0_bufferLog.txt' using 1:2  title "tobasco" lw 1.2 ps 1.2 

set terminal png truecolor size 1280,720   
set output "bitrateLevel.png"  
set autoscale   
set style data linespoints
set xlabel "Time/s" 
set ylabel "RepIndex"
set title "RepIndex" 
set grid
set key left
plot [0:320] [0:10] './tobasco/6/sim3_cl0_adaptationLog.txt' using 3:2  title "tobasco" lw 1.2 ps 1.2 

set terminal png truecolor size 1280,720   
set output "BandwidthEstimate.png"  
set autoscale   
set style data linespoints
set xlabel "Time/s" 
set ylabel "BandwidthEstimate/bps"
set title "BandwidthEstimate" 
set grid
set key left
plot [0:320] [0:150000000] './tobasco/6/sim3_cl0_adaptationLog.txt' using 3:4  title "tobasco" lw 1.2 ps 1.2 

set terminal png truecolor size 1280,720   
set output "Playback.png"  
set autoscale   
set style data linespoints 
set xlabel "Time/s" 
set ylabel "RepIndex"
set title "Playback" 
set grid
set key left
plot [0:320] [0:10] './tobasco/6/sim3_cl0_playbackLog.txt' using 2:3  title "tobasco" lw 1.2 ps 1.2 

set terminal png truecolor size 1280,720  
set output "PauseTime-tobasco.png"  
set autoscale   
set style data impulses 
set xlabel "Time/s" 
set ylabel "PauseTime/ms"
set title "PauseTime" 
set grid
set key left
plot [0:320] [0:3000] './tobasco/6/sim3_cl0_adaptationLog.txt' using 3:5  title "tobasco" lw 4
