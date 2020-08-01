import { Component, OnInit, ViewChild, AfterViewInit } from '@angular/core';
import { MenuItem, MessageService } from 'primeng/api';
import { StoreService } from './store/store.service';
import { Editor } from 'primeng/editor';
import * as _ from 'lodash';
import { GrblService } from './services/grbl.service';
import { Sema } from 'async-sema';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
  providers: [
    MessageService
  ]
})
export class AppComponent implements OnInit, AfterViewInit {
  title = 'ui';
  items: MenuItem[];
  display: boolean = false;
  text: string;
  public progress = 0;
  public filename = "test.gcode";

  public quill: any;
  @ViewChild(Editor) editorComponent: Editor;

  constructor(
    private grblService: GrblService,
    private storeService: StoreService,
    private messageService: MessageService
  ) {
  }

  ngOnInit() {
    this.items = [
      {
        label: 'Files',
        items: [{
          label: 'Write',
          icon: 'pi pi-fw pi-file-o',
          command: () => {
            this.display = true;
          }
        }
        ]
      },
      {
        label: 'System',
        items: [{
          label: 'Reboot',
          icon: 'pi pi-fw pi-power-off',
          command: () => {
            this.grblService.reboot().toPromise().then(() => {
            }).catch(err => {
              this.messageService.add({ severity: 'info', summary: 'Command', detail: 'reboot' });
            });
          }
        },
        {
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

  ngAfterViewInit() {
    this.quill = this.editorComponent.quill;
  }

  write() {
    const s = new Sema(
      4, // Allow 4 concurrent async calls
      {
        capacity: 100 // Prealloc space for 100 tokens
      }
    );

    let offset = 0;
    let data = this.quill.getText();
    let maxLine = data.split('\n').length;
    let maxData = data.length;
    let curData = 0;
    let promises = _.map(data.split('\n'), (line: string) => {
      return new Promise(async (resolve) => {
        offset++;
        await s.acquire()
        this.grblService.writeFile('/' + this.filename, line, offset === 0 ? true : false).toPromise().then((result: any) => {
          curData += result.written + 2;
          this.progress = Math.trunc(curData * 100 / maxData);
          s.release();
          resolve(result.written + 2);
        });
      });
    });
    Promise.all(promises).then((values) => {
      this.progress = 100;
    });
  }

  private command(command: string) {
    this.messageService.add({ severity: 'info', summary: 'Command', detail: command });
    this.storeService.dispatchAddCommand(command);
  }
}
