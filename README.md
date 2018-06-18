# miniDFS
Simple version of Distributed File System

## Design a Mini Distributed File System (Mini-DFS), which contains
- A client
- A name server
- Four data servers
![img](http://www.cs.sjtu.edu.cn/~wuct/bdpt/pic/project4_pic1.png)

## Mini-DFS is running through a process. In this process, the name server and data servers are different threads
## Basic functions of Mini-DFS
- Read/write a file
- Upload a file: upload success and return the ID of the file
- Read the location of a file based on the file ID and the offset
## File striping
- Slicing a file into several chunks
- Each chunk is 2MB
- Uniform distribution of these chunks among four data servers
## Replication
- Each chunk has three replications
- Replicas are distributed in different data servers
## Name Server
- List the relationships between file and chunks
- List the relationships between replicas and data servers
- Data server management
## Data Server
- Read/Write a local chunk
- Write a chunk via a local directory path
## Client
- Provide read/write interfaces of a file
## Mini-DFS can show
- Read a file (more than 7MB)
- Via input the file and directory
- Write a file (more than 3MB)
- Each data server should contain appropriate number of chunks
- Using MD5 checksum for a chunk in different data servers, the results should be the same
- Check a file in (or not in) Mini-DFS via inputting a given directory
- By inputting a file and a random offset, output the content

# compile
```bash
make
```

# run
```bash
./miniDFS
put source_file_path dest_file_path
ls
locate FileID Offset
fetch FileID Offset dest_file_path
...
read source_file_path dest_file_path
...
```
