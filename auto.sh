##
#文件名: auto.sh
#作者: 李程鹏 on Mar 19, 2018
#描述: 自动生成线程测试文件
##

concurrency=(100 500 1000)
threads=(50 100 200 100 500 1000 200 1000 2000)
requests=(1000 2000 3000 4000 5000 6000 7000 8000 9000 10000)

for i in 0 1 2
do
	for j in 0 1 2
	do
		for k in ${requests[@]}
		do
			./happyTester -n ${k} -c ${concurrency[i]} -t ${threads[i*3+j]} -f http://localhost:98/ >> doc/explores/explore${i}.txt
		done
	done
done

for i in 0 1 2
do
	for j in 0 1 2
	do
		for k in ${requests[@]}
		do
			./happyTester -n ${k} -c ${concurrency[i]} -t ${threads[i*3+j]} -f http://localhost:80/ >> doc/explores/explore_apache${i}.txt
		done
	done
done