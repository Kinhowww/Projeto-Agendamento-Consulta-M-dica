#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define MAX_CPF_LENGTH 15
#define MAX_PASSWORD_LENGTH 20
#define MAX_APPOINTMENTS 100
#define MAX_TIMESLOTS 5

struct User {
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char cpf[MAX_CPF_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
};

struct Appointment {
    char doctor[20];
    char date[20];
    char time[10];
};

struct User users[MAX_USERS];
struct Appointment appointments[MAX_APPOINTMENTS];
int numUsers = 0;
int numAppointments = 0;

const char* timeSlots[MAX_TIMESLOTS] = {"9:00", "10:00", "11:00", "14:00", "15:00"};
bool slotAvailable[MAX_TIMESLOTS] = {true, true, true, true, true};

void saveUsersToFile() {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < numUsers; i++) {
        fprintf(file, "%s %s %s %s\n", users[i].name, users[i].email, users[i].cpf, users[i].password);
    }

    fclose(file);
}

void loadUsersFromFile() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Arquivo de usuarios não encontrado.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s", users[numUsers].name, users[numUsers].email, users[numUsers].cpf, users[numUsers].password) == 4) {
        numUsers++;
        if (numUsers >= MAX_USERS) {
            printf("Limite de usuarios excedido.\n");
            break;
        }
    }

    fclose(file);
}

void saveAppointmentsToFile() {
    FILE *file = fopen("appointments.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < numAppointments; i++) {
        fprintf(file, "%s %s %s\n", appointments[i].doctor, appointments[i].date, appointments[i].time);
    }

    fclose(file);
}

void loadAppointmentsFromFile() {
    FILE *file = fopen("appointments.txt", "r");
    if (file == NULL) {
        printf("Arquivo de agendamentos nao encontrado.\n");
        return;
    }

    while (fscanf(file, "%s %s %s", appointments[numAppointments].doctor, appointments[numAppointments].date, appointments[numAppointments].time) == 3) {
        numAppointments++;
        if (numAppointments >= MAX_APPOINTMENTS) {
            printf("Limite de agendamentos excedido.\n");
            break;
        }
    }

    fclose(file);
}

void registerUser() {
    if (numUsers >= MAX_USERS) {
        printf("Limite de usuarios atingido.\n");
        return;
    }

    printf("Cadastro de Usuario\n");
    printf("Nome: ");
    scanf("%s", users[numUsers].name);
    printf("Email: ");
    scanf("%s", users[numUsers].email);

    // Validar e capturar CPF
    bool validCPF = false;
    while (!validCPF) {
        printf("CPF (somente numeros): ");
        scanf("%s", users[numUsers].cpf);
        // Verificar se o CPF contém apenas dígitos
        validCPF = true;
        for (int i = 0; i < strlen(users[numUsers].cpf); i++) {
            if (!isdigit(users[numUsers].cpf[i])) {
                validCPF = false;
                printf("CPF invalido. Informe apenas numeros.\n");
                break;
            }
        }
    }

    printf("Senha: ");
    scanf("%s", users[numUsers].password);

    numUsers++;
    saveUsersToFile();
    printf("Usuario cadastrado com sucesso.\n");
}

void loginUser() {
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int loggedIn = 0;

    printf("Login\n");
    printf("Email: ");
    scanf("%s", email);
    printf("Senha: ");
    scanf("%s", password);

    for (int i = 0; i < numUsers; i++) {
        if (strcmp(email, users[i].email) == 0 && strcmp(password, users[i].password) == 0) {
            printf("Login bem-sucedido. Bem vindo, %s!\n", users[i].name);
            loggedIn = 1;
            break;
        }
    }

    if (!loggedIn) {
        printf("Email ou senha incorretos.\n");
    }
}

void displayDoctors() {
    printf("Medicos Disponiveis:\n");
    printf("1. Pediatra\n");
    printf("2. Psicologo\n");
    printf("3. Oftalmologista\n");
    printf("4. Neurologista\n");
    printf("5. Dermatologista\n");
}

void displayTimeSlots() {
    printf("Horarios Disponiveis:\n");
    for (int i = 0; i < MAX_TIMESLOTS; i++) {
        if (slotAvailable[i]) {
            printf("%d. %s\n", i + 1, timeSlots[i]);
        }
    }
}

void bookAppointment() {
    int doctorChoice, timeChoice;
    printf("\nAgendar Consulta\n");
    displayDoctors();
    printf("Escolha o medico desejado: ");
    scanf("%d", &doctorChoice);
    if (doctorChoice < 1 || doctorChoice > 5) {
        printf("Escolha invalida.\n");
        return;
    }
    displayTimeSlots();
    printf("Escolha o horario desejado: ");
    scanf("%d", &timeChoice);
    if (timeChoice < 1 || timeChoice > MAX_TIMESLOTS || !slotAvailable[timeChoice - 1]) {
        printf("Horário invalido ou indisponivel.\n");
        return;
    }

    strcpy(appointments[numAppointments].doctor, "");
    switch (doctorChoice) {
        case 1:
            strcpy(appointments[numAppointments].doctor, "Pediatra");
            break;
        case 2:
            strcpy(appointments[numAppointments].doctor, "Psicologo");
            break;
        case 3:
            strcpy(appointments[numAppointments].doctor, "Oftalmologista");
            break;
        case 4:
            strcpy(appointments[numAppointments].doctor, "Neurologista");
            break;
        case 5:
            strcpy(appointments[numAppointments].doctor, "Dermatologista");
            break;
        default:
            break;
    }

    strcpy(appointments[numAppointments].time, timeSlots[timeChoice - 1]);
    slotAvailable[timeChoice - 1] = false;
    numAppointments++;
    saveAppointmentsToFile();
    printf("Consulta agendada com sucesso.\n");
}

void logoutUser() {
    printf("Usuario deslogado.\n");
}

void changePassword(char email[MAX_EMAIL_LENGTH]) {
    char cpf[MAX_CPF_LENGTH];
    char newPassword[MAX_PASSWORD_LENGTH];
    int userIndex = -1;

    // Procurar o usuário pelo email
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(email, users[i].email) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("Usuario não encontrado.\n");
        return;
    }

    printf("Por favor, insira os 3 ultimos digitos do seu CPF para confirmar: ");
    scanf("%s", cpf);

    // Verificar se os 3 últimos dígitos do CPF coincidem
    if (strcmp(cpf, &users[userIndex].cpf[strlen(users[userIndex].cpf) - 3]) != 0) {
        printf("CPF incorreto. Tente novamente.\n");
        return;
    }

    printf("Digite a nova senha: ");
    scanf("%s", newPassword);
    strcpy(users[userIndex].password, newPassword);

    printf("Senha alterada com sucesso.\n");
    saveUsersToFile();
}

int main() {
    loadUsersFromFile();
    loadAppointmentsFromFile();

    int choice;
    bool loggedIn = false;
    char userEmail[MAX_EMAIL_LENGTH];

    do {
        printf("\nMenu:\n");
        if (!loggedIn) {
            printf("1. Cadastro\n");
            printf("2. Login\n");
        } else {
            printf("3. Agendar Consulta\n");
            printf("4. Trocar Senha\n");
            printf("5. Deslogar\n");
        }
        printf("6. Sair\n");
        printf("Escolha: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                loggedIn = true;
                break;
            case 3:
                if (loggedIn) {
                    bookAppointment();
                } else {
                    printf("Faça o login primeiro.\n");
                }
                break;
            case 4:
                if (loggedIn) {
                    printf("Digite o email da sua conta: ");
                    scanf("%s", userEmail);
                    changePassword(userEmail);
                } else {
                    printf("Nenhuma conta esta logada.\n");
                }
                break;
            case 5:
                if (loggedIn) {
                    logoutUser();
                    loggedIn = false;
                } else {
                    printf("Nenhuma conta esta logada.\n");
                }
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção invalida. Por favor, escolha novamente.\n");
        }
    } while (choice != 6);

    return 0;
}