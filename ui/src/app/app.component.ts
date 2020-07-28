import { Component, OnInit } from '@angular/core';
import { MenuItem, MessageService } from 'primeng/api';
import { StoreService } from './store/store.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
  providers: [
    MessageService
  ]
})
export class AppComponent implements OnInit {
  title = 'ui';
  items: MenuItem[];

  constructor(
    private storeService: StoreService,
    private messageService: MessageService
  ) {
  }

  ngOnInit() {
    this.items = [
      {
        label: 'System',
        items: [{
          label: 'Envoyer Ok',
          icon: 'pi pi-fw pi-thumbs-up',
          command: () => {
            this.command('ok');
          }
        },
        {
          label: 'Envoyer Unlock',
          icon: 'pi pi-fw pi-unlock',
          command: () => {
            this.command('$X');
          }
        },
        {
          label: 'Envoyer Reset',
          icon: 'pi pi-fw pi-refresh',
          command: () => {
            this.command(String.fromCharCode(0x18, 0x10));
          }
        },
        {
          label: 'Envoyer Pause',
          icon: 'pi pi-fw pi-pause',
          command: () => {
            this.command('!');
          }
        },
        {
          label: 'Envoyer Resume',
          icon: 'pi pi-fw pi-sign-in',
          command: () => {
            this.command('~');
          }
        },
        {
          label: 'Envoyer Status',
          icon: 'pi pi-fw pi-question-circle',
          command: () => {
            this.command('?');
          }
        }
        ]
      },
      {
        label: 'Position',
        items: [
          {
            label: 'Set X',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('G10 L20 P1 X0');
            }
          },
          {
            label: 'Set Y',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('G10 L20 P1 Y0');
            }
          },
          {
            label: 'Set Z',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('G10 L20 P1 Z0');
            }
          },
          {
            label: 'Set All',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('G10 L20 P1 X0 Y0 Z0');
            }
          },
          {
            label: 'Set Home',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('$H');
            }
          },
          {
            label: 'Set Home defined',
            icon: 'pi pi-fw pi-cog',
            command: () => {
              this.command('G28.1');
            }
          },
        ]
      }
    ];
  }

  private command(command: string) {
    this.messageService.add({ severity: 'info', summary: 'Command', detail: command });
    this.storeService.dispatchAddCommand(command);
  }
}
