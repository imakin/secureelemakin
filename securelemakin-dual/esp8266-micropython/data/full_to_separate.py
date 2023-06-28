#WARNING: untracked secret file
#destroy data when done
import os
with open("full_to_separate_input") as f:
    data = f.read()
    if len(data)>0:
        for data_line in data.split('\n'):
            try:
                name,message = data_line.split(' ')
                with open(f"{name}.raw","w") as f:
                    f.write(message)
                    print(f"making file : {name}.raw")
            except Exception as e:
                print(e)
