import { Injectable } from '@angular/core';
import { HttpClient, HttpErrorResponse } from '@angular/common/http';
import { throwError, Observable } from 'rxjs';
import { catchError, retry, timeout } from 'rxjs/operators';
import { Status } from '../models/grbl';

export interface Config {
  heroesUrl: string;
  textfile: string;
}

@Injectable({
  providedIn: 'root'
})
export class GrblService {

  configUrl: string;

  constructor(private http: HttpClient) {
    this.configUrl = '/api/';
  }

  getStatus(): Observable<Status> {
    return this.http.get<Status>(this.configUrl + 'v1/simulate')
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  reboot(): Observable<any> {
    return this.http.get<any>(this.configUrl + 'v1/reboot')
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  browseFile(): Observable<string[]> {
    return this.http.get<string[]>(this.configUrl + 'v1/files')
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  printFile(filename: string): Observable<string> {
    return this.http.post<string>(this.configUrl + 'v1/print', {
      file: filename
    }, { responseType: 'text' as 'json' })
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  deleteFile(filename: string): Observable<string> {
    return this.http.post<string>(this.configUrl + 'v1/delete', {
      file: filename
    })
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  writeFile(filename: string, data: string, touch: boolean): Observable<string> {
    return this.http.post<string>(this.configUrl + 'v1/file', {
      file: filename,
      data,
      op: touch ? "init" : "append"
    })
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  addCommand(body: string): Observable<string> {
    return this.http.put<string>(this.configUrl + 'v1/simulate', body, { responseType: 'text' as 'json' })
      .pipe(
        retry(0), // retry a failed request up to 3 times
        catchError(this.handleError), // then handle the error
        timeout(2000)
      );
  }

  private handleError(error: HttpErrorResponse) {
    if (error.error instanceof ErrorEvent) {
      // A client-side or network error occurred. Handle it accordingly.
      console.error('An error occurred:', error.error.message);
    } else {
      // The backend returned an unsuccessful response code.
      // The response body may contain clues as to what went wrong,
      console.error(
        `Backend returned code ${error.status}, ` +
        `body was: ${error.error.text}`, error);
    }
    // return an observable with a user-facing error message
    return throwError(
      error);
  }
}
