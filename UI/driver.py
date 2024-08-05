import subprocess

# Function to interact with the C program
def interact_with_c_program():
    process = subprocess.Popen(['E:\Git\KingsIndian\KingsIndian\main_gui'], stdin=subprocess.PIPE,stdout=subprocess.PIPE, text=True)

    try:
        while True:
            # Take user input
            user_input = input("Enter your move: ")

            # Send the number to the C program
            process.stdin.write(f"{user_input}\n")
            process.stdin.flush()

            validity = process.stdout.readline().strip()
            print("Validity : "+validity)
            if validity == 'invalid':
                continue
            computer_move = process.stdout.readline().strip()
            #process.stdout.flush()
            print(f"Computer move: {computer_move}")
            
    except KeyboardInterrupt:
        print("Exiting...")

    finally:
        # Terminate the C program subprocess
        process.terminate()

# Run the function
if __name__ == "__main__":
    interact_with_c_program()
